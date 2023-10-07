// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _GL2JPEG_H_
#define _GL2JPEG_H_

#include <stdio.h>
#include "PixelBuffer.h"

void create_jpeg(FILE *outfile, PixelBuffer *buffer, int quality, int smoothing);

#endif
