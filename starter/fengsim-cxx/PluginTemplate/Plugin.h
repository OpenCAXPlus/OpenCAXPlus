// myplugin.h
#ifndef PLUGIN_H
#define PLUGIN_H

#include "PluginInterface.h"

class Plugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.example.PluginInterface")
    Q_INTERFACES(PluginInterface)

public:
    Plugin();
    virtual ~Plugin();

    virtual QString name() const;
    virtual QString content() const;
};

#endif // PLUGIN_H
