
DefineConstant[ refine = {0.1,
    Name "Parameters/Geometry/Refinement at corner"} ] ;

lc = 0.1;

a = 1.0;
b = 1.0;
h = 1.0;

Point(1) = {0, 0, 0, lc*refine};
Point(2) = {a, 0, 0, lc} ;
Point(3) = {0, b, 0, lc} ;
Point(4) = {a, -h, 0, lc} ;
Point(5) = {-h, b, 0, lc} ;
Point(6) = {-h, -h, 0, lc} ;

Line(1) = {1,3} ;
Line(2) = {3,5} ;
Line(3) = {5,6} ;
Line(4) = {6,4} ;
Line(5) = {4,2} ;
Line(6) = {2,1} ;

Line Loop(1) = {1,2,3,4,5,6} ;
Plane Surface(1) = {1};

