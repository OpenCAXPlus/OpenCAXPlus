/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkOSPRayVolumeMapperNode.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkOSPRayVolumeMapperNode
 * @brief   links vtkVolumeMapper  to OSPRay
 *
 * Translates vtkVolumeMapper state into OSPRay rendering calls
*/

#ifndef vtkOSPRayVolumeMapperNode_h
#define vtkOSPRayVolumeMapperNode_h

#include "vtkRenderingOSPRayModule.h" // For export macro
#include "vtkVolumeMapperNode.h"

class vtkAbstractArray;
class vtkDataSet;
class vtkVolume;

namespace osp
{
  struct TransferFunction;
  struct Volume;
}

class VTKRENDERINGOSPRAY_EXPORT vtkOSPRayVolumeMapperNode :
  public vtkVolumeMapperNode
{
public:
  static vtkOSPRayVolumeMapperNode* New();
  vtkTypeMacro(vtkOSPRayVolumeMapperNode, vtkVolumeMapperNode);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Make ospray calls to render me.
   */
  virtual void Render(bool prepass) override;

  /**
   * TODO: fix me
   * should be controlled by VTK SampleDistance, otherwise
   * should use macros and modify self.
   */
  void SetSamplingRate(double rate) { this->SamplingRate = rate; }
  double GetSamplingRate() { return this->SamplingRate; }

protected:
  vtkOSPRayVolumeMapperNode();
  ~vtkOSPRayVolumeMapperNode();

  /**
   * updates internal OSPRay transfer function for volume
   */
  void UpdateTransferFunction(vtkVolume* vol);

  //TODO: SetAndGetters?
  int NumColors;
  double SamplingRate;

  vtkTimeStamp BuildTime;
  vtkTimeStamp PropertyTime;

  osp::Volume* OSPRayVolume;
  osp::TransferFunction* TransferFunction;
  std::vector<float> TFVals;
  std::vector<float> TFOVals;

private:
  vtkOSPRayVolumeMapperNode(const vtkOSPRayVolumeMapperNode&) = delete;
  void operator=(const vtkOSPRayVolumeMapperNode&) = delete;
};
#endif
