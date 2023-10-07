// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _READ_IMG_
#define _READ_IMG_

#include <string>

int read_pnm(std::string fileName);
int read_jpeg(std::string fileName);
int read_png(std::string fileName);
int read_bmp(std::string fileName);

#endif
