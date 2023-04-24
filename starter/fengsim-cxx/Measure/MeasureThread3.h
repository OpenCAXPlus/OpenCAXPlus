#ifndef MEASURETHREAD3_H
#define MEASURETHREAD3_H

#include "CAD/Primitive.h"
#include "MeasureDockWidget.h"
#include "Mesh/MeshGeneration.h"
#include "Visual/VTKWidget.h"
#include "ui_MeasureDockWidget.h"
#include <QThread>

class MeasureThread3 : public QThread
{
    Q_OBJECT
public:
    Boundaries*        meas_bnds;
    MeshModule         MM;
    VTKWidget*         vtk_widget;
    MeasureDockWidget* measure_dock;
    MeasureThread3() {}
    QString path;
    void    run();
};

#endif // MEASURETHREAD3_H
