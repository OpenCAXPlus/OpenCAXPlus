#include <QString>
#include <QtPlugin>

class PluginInterface
{
public:
    virtual ~PluginInterface() {}

    virtual QString name() const    = 0;
    virtual QString content() const = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "com.example.PluginInterface")
