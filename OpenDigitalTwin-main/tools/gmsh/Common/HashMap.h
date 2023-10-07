// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

//--Define custom hash map here.  It must have form <key, value, HashFunctor,
//--EqualFunctor>

#define HASH_MAP         __gnu_cxx::hash_map
#include <ext/hash_map>

#endif
