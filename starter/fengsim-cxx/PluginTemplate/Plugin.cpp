#include "Plugin.h"

Plugin::Plugin() {}

Plugin::~Plugin() {}

QString Plugin::name() const { return "My Plugin"; }

QString Plugin::content() const { return "This is the content of My Plugin."; }
