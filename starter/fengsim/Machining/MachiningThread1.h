#ifndef MACHININGTHREAD1_H
#define MACHININGTHREAD1_H

#include <QProcess>
#include <QThread>

class MachiningThread1 : public QThread
{
    Q_OBJECT
public:
    MachiningThread1();
    void run()
    {
        QProcess* proc = new QProcess();
        proc->setWorkingDirectory("/home/jiping/FENGSim/M++/");
        QString command(QString("./MachiningRun"));
        proc->start(command);
        if (proc->waitForFinished(-1)) {
            quit();
        }
        exec();
    }
};

#endif // MACHININGTHREAD1_H
