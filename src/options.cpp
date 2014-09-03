#include "options.h"

static const char* ADDROOTPATH_ENABLED = "properties/rootpathtofilename";

Options::Options(QObject *parent) :
    QObject(parent),
     m_addRootPath(true)
{
}

void Options::readSettings()
{
     QSettings settings;

     m_addRootPath = settings.value(ADDROOTPATH_ENABLED, true).toBool();
}

void Options::writeSettings()
{
    QSettings settings;

    settings.setValue(ADDROOTPATH_ENABLED, m_addRootPath);
}
