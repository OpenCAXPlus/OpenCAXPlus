// Gmsh - Copyright (C) 1997-2017 C. Geuzaine, J.-F. Remacle
//
// See the LICENSE.txt file for license information. Please report all
// bugs and problems to the public mailing list <gmsh@onelab.info>.

#ifndef _GRAPHIC_WINDOW_H_
#define _GRAPHIC_WINDOW_H_

#include <QWidget>
#include "GLWidget.h"

class QSlider;

class graphicWindow : public QWidget
{
  Q_OBJECT
 private:
  GLWidget *_glWidget;
  QSlider *_xSlider;
 public:
  graphicWindow();
  drawContext *getDrawContext(){ return _glWidget->getDrawContext(); }
};

#endif
