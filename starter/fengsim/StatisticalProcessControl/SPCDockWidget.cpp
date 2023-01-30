#include "SPCDockWidget.h"
#include "ui_SPCDockWidget.h"

#include <QProcess>

SPCDockWidget::SPCDockWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SPCDockWidget)
{
    ui->setupUi(this);

    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(check()));
}

SPCDockWidget::~SPCDockWidget()
{
    delete ui;
}

void SPCDockWidget::check() {
    if (ui->treeWidget->currentItem()->childCount() == 0) {
        QProcess *proc = new QProcess();
        proc->setWorkingDirectory( "/home/jiping/FENGSim/FENGSim/StatisticalProcessControl/build-spc-Desktop_Qt_5_10_1_GCC_64bit-Debug/" );
        proc->start(QString("./spc") );
    }
}
