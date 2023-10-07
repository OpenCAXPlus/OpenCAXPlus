// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.
//
// Contributor(s):
//   Ruth Sabariego  & Francois Henrotte
//

#include "NewView.h"
#include "GModel.h"
#include "MElement.h"

StringXNumber NewViewOptions_Number[] = {
  {GMSH_FULLRC, "View", NULL, -1.},
  {GMSH_FULLRC, "Dimension", NULL, 1.}
};

extern "C"
{
  GMSH_Plugin *GMSH_RegisterNewViewPlugin()
  {
    return new GMSH_NewViewPlugin();
  }
}

std::string GMSH_NewViewPlugin::getHelp() const
{
  return "Plugin(NewView) creates a new view from a mesh."
    "The parameter `Dimension' gives the dimension"
    "of the initialized to zero NodeData vector";
}

int GMSH_NewViewPlugin::getNbOptions() const
{
  return sizeof(NewViewOptions_Number) / sizeof(StringXNumber);
}

StringXNumber *GMSH_NewViewPlugin::getOption(int iopt)
{
  return &NewViewOptions_Number[iopt];
}

PView *GMSH_NewViewPlugin::execute(PView * v)
{
  if(GModel::current()->getMeshStatus() < 1){
    Msg::Error("No mesh available to create the view: please mesh your model!");
    return v ;
  }
  int nbrValues = (int)NewViewOptions_Number[1].def;

  std::map<int, std::vector<double> > d;
  std::vector<GEntity*> entities;
  GModel::current()->getEntities(entities);
  for(unsigned int i = 0; i < entities.size(); i++){
    for(unsigned int j = 0; j < entities[i]->mesh_vertices.size(); j++){
      MVertex *ve = entities[i]->mesh_vertices[j];
      d[ve->getNum()].resize(nbrValues);
    }
  }

  PView *vn = new PView("New view", "NodeData", GModel::current(), d);
  return vn ;
}
