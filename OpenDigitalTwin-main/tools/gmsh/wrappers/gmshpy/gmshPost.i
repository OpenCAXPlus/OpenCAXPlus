%feature("autodoc", "1");
%module gmshPost
%include typemaps.i
%include std_string.i
%include std_vector.i
%include std_map.i
%import "gmshtypemaps.i"

%{
  #undef HAVE_DLOPEN
  #include "GmshConfig.h"
  #include "fullMatrix.h"
#if defined(HAVE_POST)
  #include "PluginManager.h"
  #include "Plugin.h"
  #include "PView.h"
  #include "PViewFactory.h"
  #include "PViewData.h"
  #include "PViewAsSimpleFunction.h"
  #include "PViewDataGModel.h"
#endif
%}

namespace std {
  %template(DoubleVector) std::vector<double, std::allocator<double> >;
  %template(IntDoubleVectorMap) std::map < int , std::vector<double, std::allocator<double> > >;
}


%include "GmshConfig.h"
#if defined(HAVE_POST)
%include "PView.h"
%include "PViewFactory.h"
%apply double &OUTPUT { double &val}
%include "PViewData.h"
%include "simpleFunction.h"
%template(simpleFunctionDouble) simpleFunction<double>;
%include "PViewAsSimpleFunction.h"
%include "PViewDataGModel.h"
%include "Plugin.h"
%include "PluginManager.h"
#endif

