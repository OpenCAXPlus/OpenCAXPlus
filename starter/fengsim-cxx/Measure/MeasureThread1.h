#ifndef MEASURETHREAD1_H
#define MEASURETHREAD1_H

#include "Visual/VTKWidget.h"
#include "fstream"
#include <QProcess>
#include <QTextEdit>
#include <QThread>

class MeasureThread1 : public QThread
{
    Q_OBJECT
public:
    VTKWidget* vtk_widget;
    MeasureThread1() {}
    void        run();
    std::string name;
    QString     path;
};

#endif // MEASURETHREAD1_H
