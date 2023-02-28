#ifndef _TEST_H_
#define _TEST_H_

#include <iostream>
#include <portage/intersect/intersect_r2d.h>
#include <portage/search/search_kdtree.h>

// wonton includes
#include "wonton/mesh/jali/jali_mesh_wrapper.h"
#include "wonton/mesh/simple/simple_mesh.h"
#include "wonton/mesh/simple/simple_mesh_wrapper.h"
#include "wonton/state/simple/simple_state.h"
#include "wonton/state/simple/simple_state_wrapper.h"
#include "wonton/support/Point.h"
#include "wonton/support/wonton.h"

// portage includes
#include "portage/driver/coredriver.h"
#include "portage/interpolate/interpolate_1st_order.h"
#include "portage/interpolate/interpolate_2nd_order.h"
#include "portage/support/portage.h"
#include "simple_intersect_for_tests.h"

#include "JaliState.h"
#include "JaliStateVector.h"

#include <fstream>

void export_vtk(int n, std::string filename, std::vector<double> val);
void export_vtk(Jali::UniStateVector<double, Jali::Mesh> cellvecout);
void export_overlap(const Wonton::Simple_Mesh_Wrapper& source,
                    std::vector<int>                   candidates,
                    const Wonton::Simple_Mesh_Wrapper& target,
                    int                                cell_id);
void export_overlap(const Wonton::Jali_Mesh_Wrapper& source,
                    std::vector<int>                 candidates,
                    const Wonton::Jali_Mesh_Wrapper& target,
                    int                              cell_id);
void export_vtk(std::vector<std::vector<Wonton::Point<2>>>& mesh,
                std::vector<double>&                        cellvecout);
void export_vtk(Jali::UniStateVector<double, Jali::Mesh> cellvecout,
                const Wonton::Jali_Mesh_Wrapper&         targetMeshWrapper);

void test_search();
void test_intersection();
void test_interpolation();
void test_remapping();

#endif
