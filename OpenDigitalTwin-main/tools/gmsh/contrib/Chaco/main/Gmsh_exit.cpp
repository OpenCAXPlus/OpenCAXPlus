// Gmsh - Copyright (C) 1997-2008 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#include <stdexcept>

extern "C" void Gmsh_exit()
{
   throw std::runtime_error("Library Chaco wants to exit");
}
