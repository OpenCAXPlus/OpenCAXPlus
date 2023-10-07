/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSPHInterpolator.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkSPHInterpolator.h"

#include "vtkObjectFactory.h"
#include "vtkSPHQuinticKernel.h"
#include "vtkVoronoiKernel.h"
#include "vtkAbstractPointLocator.h"
#include "vtkArrayListTemplate.h"
#include "vtkStaticPointLocator.h"
#include "vtkDataSet.h"
#include "vtkDataArray.h"
#include "vtkImageData.h"
#include "vtkPoints.h"
#include "vtkCharArray.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"
#include "vtkCellData.h"
#include "vtkPointData.h"
#include "vtkIdList.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkMath.h"
#include "vtkSMPTools.h"
#include "vtkSMPThreadLocalObject.h"

vtkStandardNewMacro(vtkSPHInterpolator);
vtkCxxSetObjectMacro(vtkSPHInterpolator,Locator,vtkAbstractPointLocator);
vtkCxxSetObjectMacro(vtkSPHInterpolator,Kernel,vtkSPHKernel);

//----------------------------------------------------------------------------
// Helper classes to support efficient computing, and threaded execution.
namespace {
// The threaded core of the algorithm
struct ProbePoints
{
  vtkSPHInterpolator *SPHInterpolator;
  vtkDataSet *Input;
  vtkSPHKernel *Kernel;
  vtkAbstractPointLocator *Locator;
  vtkPointData *InPD;
  vtkPointData *OutPD;
  ArrayList Arrays;
  ArrayList DerivArrays;
  bool ComputeDerivArrays;
  char *Valid;
  int Strategy;
  float *Shepard;
  bool Promote;

  // Don't want to allocate these working arrays on every thread invocation,
  // so make them thread local.
  vtkSMPThreadLocalObject<vtkIdList> PIds;
  vtkSMPThreadLocalObject<vtkDoubleArray> Weights;
  vtkSMPThreadLocalObject<vtkDoubleArray> DerivWeights;

  ProbePoints(vtkSPHInterpolator *sphInt, vtkDataSet *input,
              vtkPointData *inPD, vtkPointData *outPD,
              char *valid, float *shepCoef) :
    SPHInterpolator(sphInt), Input(input), InPD(inPD), OutPD(outPD),
    Valid(valid), Shepard(shepCoef)
  {
      // Gather information from the interpolator
      this->Kernel = sphInt->GetKernel();
      this->Locator = sphInt->GetLocator();
      this->Strategy = sphInt->GetNullPointsStrategy();
      double nullV = sphInt->GetNullValue();
      this->Promote = sphInt->GetPromoteOutputArrays();

      // Manage arrays for interpolation
      for (int i=0; i < sphInt->GetNumberOfExcludedArrays(); ++i)
      {
        const char *arrayName = sphInt->GetExcludedArray(i);
        vtkDataArray *array = this->InPD->GetArray(arrayName);
        if ( array != nullptr )
        {
          outPD->RemoveArray(array->GetName());
          this->Arrays.ExcludeArray(array);
          this->DerivArrays.ExcludeArray(array);
        }
      }
      this->Arrays.AddArrays(input->GetNumberOfPoints(), inPD, outPD, nullV, this->Promote);

      // Sometimes derivative arrays are requested
      for (int i=0; i < sphInt->GetNumberOfDerivativeArrays(); ++i)
      {
        const char *arrayName = sphInt->GetDerivativeArray(i);
        vtkDataArray *array = this->InPD->GetArray(arrayName);
        if ( array != nullptr )
        {
          vtkStdString outName = arrayName; outName += "_deriv";
          if (vtkDataArray* outArray = this->DerivArrays.AddArrayPair(
                array->GetNumberOfTuples(), array, outName, nullV, this->Promote))
          {
            outPD->AddArray(outArray);
          }
        }
      }
      this->ComputeDerivArrays = (!this->DerivArrays.Arrays.empty() ? true : false);
  }

  // Just allocate a little bit of memory to get started.
  void Initialize()
  {
    vtkIdList*& pIds = this->PIds.Local();
    pIds->Allocate(128); //allocate some memory
    vtkDoubleArray*& weights = this->Weights.Local();
    weights->Allocate(128);
    vtkDoubleArray*& gradWeights = this->DerivWeights.Local();
    gradWeights->Allocate(128);
  }

  // Threaded interpolation method
  void operator() (vtkIdType ptId, vtkIdType endPtId)
  {
      double x[3];
      vtkIdList*& pIds = this->PIds.Local();
      vtkIdType numWeights;
      vtkDoubleArray*& weights = this->Weights.Local();
      vtkDoubleArray*& gradWeights = this->DerivWeights.Local();

      for ( ; ptId < endPtId; ++ptId)
      {
        this->Input->GetPoint(ptId,x);

        if ( (numWeights=this->Kernel->ComputeBasis(x, pIds, ptId)) > 0 )
        {
          if ( ! this->ComputeDerivArrays )
          {
            this->Kernel->ComputeWeights(x, pIds, weights);
          }
          else
          {
            this->Kernel->ComputeDerivWeights(x, pIds, weights, gradWeights);
            this->DerivArrays.Interpolate(numWeights, pIds->GetPointer(0),
                                          gradWeights->GetPointer(0), ptId);
          }
          this->Arrays.Interpolate(numWeights, pIds->GetPointer(0),
                                   weights->GetPointer(0), ptId);
        }
        else // no neighborhood points
        {
          this->Arrays.AssignNullValue(ptId);
          if ( this->Strategy == vtkSPHInterpolator::MASK_POINTS)
          {
            this->Valid[ptId] = 0;
          }
        }// null point

        // Shepard's coefficient if requested
        if ( this->Shepard )
        {
          double sum=0.0, *w=weights->GetPointer(0);
          for (int i=0; i < numWeights; ++i)
          {
            sum += w[i];
          }
          this->Shepard[ptId] = sum;
        }
      }//for all dataset points
  }

  void Reduce()
  {
  }

}; //ProbePoints
} //anonymous namespace

//================= Begin class proper =======================================
//----------------------------------------------------------------------------
vtkSPHInterpolator::vtkSPHInterpolator()
{
  this->SetNumberOfInputPorts(2);

  this->Locator = vtkStaticPointLocator::New();

  this->Kernel = vtkSPHQuinticKernel::New();

  this->CutoffArrayName = "";

  this->DensityArrayName = "Rho";
  this->MassArrayName = "";

  this->NullPointsStrategy = vtkSPHInterpolator::NULL_VALUE;
  this->NullValue = 0.0;

  this->ValidPointsMask = nullptr;
  this->ValidPointsMaskArrayName = "vtkValidPointMask";

  this->ComputeShepardSum = true;
  this->ShepardSumArrayName = "Shepard Summation";

  this->PromoteOutputArrays = true;

  this->PassPointArrays = true;
  this->PassCellArrays = true;
  this->PassFieldArrays = true;
}

//----------------------------------------------------------------------------
vtkSPHInterpolator::~vtkSPHInterpolator()
{
  this->SetLocator(nullptr);
  this->SetKernel(nullptr);
}

//----------------------------------------------------------------------------
void vtkSPHInterpolator::SetSourceConnection(vtkAlgorithmOutput* algOutput)
{
  this->SetInputConnection(1, algOutput);
}

//----------------------------------------------------------------------------
void vtkSPHInterpolator::SetSourceData(vtkDataObject *input)
{
  this->SetInputData(1, input);
}

//----------------------------------------------------------------------------
vtkDataObject *vtkSPHInterpolator::GetSource()
{
  if (this->GetNumberOfInputConnections(1) < 1)
  {
    return nullptr;
  }

  return this->GetExecutive()->GetInputData(1, 0);
}

//----------------------------------------------------------------------------
// The driver of the algorithm
void vtkSPHInterpolator::
Probe(vtkDataSet *input, vtkDataSet *source, vtkDataSet *output)
{
  // Make sure there is a kernel
  if ( !this->Kernel )
  {
    vtkErrorMacro(<<"Interpolation kernel required\n");
    return;
  }

  // Start by building the locator
  if ( !this->Locator )
  {
    vtkErrorMacro(<<"Point locator required\n");
    return;
  }
  this->Locator->SetDataSet(source);
  this->Locator->BuildLocator();

  // Set up the interpolation process
  vtkIdType numPts = input->GetNumberOfPoints();
  vtkPointData *sourcePD = source->GetPointData();
  vtkPointData *outPD = output->GetPointData();
  outPD->InterpolateAllocate(sourcePD,numPts);

  // Masking if requested
  char *mask=nullptr;
  if ( this->NullPointsStrategy == vtkSPHInterpolator::MASK_POINTS )
  {
    this->ValidPointsMask = vtkCharArray::New();
    this->ValidPointsMask->SetNumberOfTuples(numPts);
    mask = this->ValidPointsMask->GetPointer(0);
    std::fill_n(mask, numPts, 1);
  }

  // Shepard summation if requested
  float *shepardArray=nullptr;
  if ( this->ComputeShepardSum )
  {
    this->ShepardSumArray = vtkFloatArray::New();
    this->ShepardSumArray->SetNumberOfTuples(numPts);
    shepardArray=this->ShepardSumArray->GetPointer(0);
  }

  // Initialize the SPH kernel
  if ( this->Kernel->GetRequiresInitialization() )
  {
    this->Kernel->SetCutoffArray(sourcePD->GetArray(this->CutoffArrayName));
    this->Kernel->SetDensityArray(sourcePD->GetArray(this->DensityArrayName));
    this->Kernel->SetMassArray(sourcePD->GetArray(this->MassArrayName));
    this->Kernel->Initialize(this->Locator, source, sourcePD);
  }

  // Now loop over input points, finding closest points and invoking kernel.
  ProbePoints probe(this, input, sourcePD, outPD, mask, shepardArray);
  vtkSMPTools::For(0, numPts, probe);

  // Clean up
  if ( this->ShepardSumArray )
  {
    this->ShepardSumArray->SetName(this->ShepardSumArrayName);
    outPD->AddArray(this->ShepardSumArray);
    this->ShepardSumArray->Delete();
    this->ShepardSumArray = nullptr;
  }

  if ( mask )
  {
    this->ValidPointsMask->SetName(this->ValidPointsMaskArrayName);
    outPD->AddArray(this->ValidPointsMask);
    this->ValidPointsMask->Delete();
    this->ValidPointsMask = nullptr;
  }
}

//----------------------------------------------------------------------------
void vtkSPHInterpolator::
PassAttributeData(vtkDataSet* input, vtkDataObject* vtkNotUsed(source),
                  vtkDataSet* output)
{
  // copy point data arrays
  if (this->PassPointArrays)
  {
    int numPtArrays = input->GetPointData()->GetNumberOfArrays();
    for (int i=0; i<numPtArrays; ++i)
    {
      output->GetPointData()->AddArray(input->GetPointData()->GetArray(i));
    }
  }

  // copy cell data arrays
  if (this->PassCellArrays)
  {
    int numCellArrays = input->GetCellData()->GetNumberOfArrays();
    for (int i=0; i<numCellArrays; ++i)
    {
      output->GetCellData()->AddArray(input->GetCellData()->GetArray(i));
    }
  }

  if (this->PassFieldArrays)
  {
    // nothing to do, vtkDemandDrivenPipeline takes care of that.
  }
  else
  {
    output->GetFieldData()->Initialize();
  }
}

//----------------------------------------------------------------------------
int vtkSPHInterpolator::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  vtkDebugMacro(<< "Executing SPH Interpolator");

  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *sourceInfo = inputVector[1]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkDataSet *input = vtkDataSet::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkDataSet *source = vtkDataSet::SafeDownCast(
    sourceInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkDataSet *output = vtkDataSet::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  if (!source || source->GetNumberOfPoints() < 1 )
  {
    vtkWarningMacro(<<"No source points to interpolate from");
    return 1;
  }

  // Copy the input geometry and topology to the output
  output->CopyStructure(input);

  // Perform the probing
  this->Probe(input, source, output);

  // Pass attribute data as requested
  this->PassAttributeData(input, source, output);

  return 1;
}

//----------------------------------------------------------------------------
int vtkSPHInterpolator::RequestInformation(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *sourceInfo = inputVector[1]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  outInfo->CopyEntry(sourceInfo,
                     vtkStreamingDemandDrivenPipeline::TIME_STEPS());
  outInfo->CopyEntry(sourceInfo,
                     vtkStreamingDemandDrivenPipeline::TIME_RANGE());

  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
               inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT()),
               6);

  // Make sure that the scalar type and number of components
  // are propagated from the source not the input.
  if (vtkImageData::HasScalarType(sourceInfo))
  {
    vtkImageData::SetScalarType(vtkImageData::GetScalarType(sourceInfo),
                                outInfo);
  }
  if (vtkImageData::HasNumberOfScalarComponents(sourceInfo))
  {
    vtkImageData::SetNumberOfScalarComponents(
      vtkImageData::GetNumberOfScalarComponents(sourceInfo),
      outInfo);
  }

  return 1;
}

//----------------------------------------------------------------------------
int vtkSPHInterpolator::RequestUpdateExtent(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *sourceInfo = inputVector[1]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER(), 0);
  inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES(), 1);
  inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS(), 0);
  sourceInfo->Set(
    vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER(),
    outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER()));
  sourceInfo->Set(
    vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES(),
    outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES()));
  sourceInfo->Set(
    vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS(),
    outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS()));
  sourceInfo->Set(
    vtkStreamingDemandDrivenPipeline::UPDATE_EXTENT(),
    sourceInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT()),
    6);

  return 1;
}

//--------------------------------------------------------------------------
vtkMTimeType vtkSPHInterpolator::GetMTime()
{
  vtkMTimeType mTime=this->Superclass::GetMTime();
  vtkMTimeType mTime2;
  if ( this->Locator != nullptr )
  {
    mTime2 = this->Locator->GetMTime();
    mTime = ( mTime2 > mTime ? mTime2 : mTime );
  }
  if ( this->Kernel != nullptr )
  {
    mTime2 = this->Kernel->GetMTime();
    mTime = ( mTime2 > mTime ? mTime2 : mTime );
  }
  return mTime;
}

//----------------------------------------------------------------------------
void vtkSPHInterpolator::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkDataObject *source = this->GetSource();

  this->Superclass::PrintSelf(os,indent);
  os << indent << "Source: " << source << "\n";
  os << indent << "Locator: " << this->Locator << "\n";
  os << indent << "Kernel: " << this->Kernel << "\n";

  os << indent << "Cutoff Array Name: " << this->CutoffArrayName << "\n";

  os << indent << "Density Array Name: " << this->DensityArrayName << "\n";
  os << indent << "Mass Array Name: " << this->MassArrayName << "\n";

  os << indent << "Null Points Strategy: " << this->NullPointsStrategy << endl;
  os << indent << "Null Value: " << this->NullValue << "\n";
  os << indent << "Valid Points Mask Array Name: "
     << (this->ValidPointsMaskArrayName ? this->ValidPointsMaskArrayName : "(none)") << "\n";

  os << indent << "Compute Shepard Sum: "
     << (this->ComputeShepardSum ? "On" : " Off") << "\n";
  os << indent << "Shepard Sum Array Name: "
     << (this->ShepardSumArrayName ? this->ShepardSumArrayName : "(none)") << "\n";

  os << indent << "Promote Output Arrays: "
     << (this->PromoteOutputArrays ? "On" : " Off") << "\n";

  os << indent << "Pass Point Arrays: "
     << (this->PassPointArrays? "On" : " Off") << "\n";
  os << indent << "Pass Cell Arrays: "
     << (this->PassCellArrays? "On" : " Off") << "\n";
  os << indent << "Pass Field Arrays: "
     << (this->PassFieldArrays? "On" : " Off") << "\n";
}
