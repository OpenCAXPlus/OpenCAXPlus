#include "app.h"

#include <BRepGProp.hxx>
#include <BRepTools.hxx>
#include <BRep_Builder.hxx>
#include <GProp_GProps.hxx>
#include <TopoDS_Shape.hxx>
#include <iostream>

int app(int argc, char** argv)
{
    TopoDS_Shape shape;
    BRep_Builder builder;
    GProp_GProps prop;

    if (argc < 2) {
        std::cerr << "Usage: computeSurface file.brep" << std::endl;
        exit(1);
    }
    BRepTools::Read(shape, argv[1], builder);
    BRepGProp::SurfaceProperties(shape, prop);
    std::cout << "Total surface of all shapes: " << prop.Mass() << std::endl;
    return 0;
}
