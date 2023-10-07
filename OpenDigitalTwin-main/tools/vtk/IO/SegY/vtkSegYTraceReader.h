/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSegYTraceReader.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef vtkSegYTraceReader_h
#define vtkSegYTraceReader_h

#include <fstream>
#include <vector>

#include "vtkSegYTraceHeaderBytesPositions.h"

/*
 * Single Seg-Y trace
 */
class vtkSegYTrace
{
public:
  float xCoordinate;
  float yCoordinate;
  short CoordinateMultiplier;
  std::vector<float> data;
  int inlineNumber;
  int crosslineNumber;
  short SampleInterval;
};

/*
 * Single Seg-Y trace reader
 */
class vtkSegYTraceReader
{
private:
  vtkSegYTraceHeaderBytesPositions traceHeaderBytesPos;

  int XCoordinate;
  int YCoordinate;

public:
  vtkSegYTraceReader();

  void SetXYCoordBytePositions(int x, int y);
  void PrintTraceHeader(std::ifstream& in, int startPos);
  bool ReadTrace(int& startPos,
    std::ifstream& in,
    int formatCode,
    vtkSegYTrace* trace);
  int GetTraceSize(int numSamples, int formatCode);
};

#endif // vtkSegYTraceReader_h
// VTK-HeaderTest-Exclude: vtkSegYTraceReader.h
