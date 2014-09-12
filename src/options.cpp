#include "options.h"

static const char* ADDROOTPATH_ENABLED = "properties/rootpathtofilename";
static const char* DEFAULTHASH = "properties/defaulthash";
static const char* ROOTPATHTYP = "properties/rootpathtyp";
static const char* FORMAT = "properties/format";

Options::Options(QObject *parent) :
    QObject(parent),
    m_addRootPath(true),
    m_defaultHash("SHA256"),
    m_rootPathTyp("dynamic"),
    m_format("default")
{
}

void Options::readSettings()
{
     QSettings settings;

     //settings.remove("properties");

     m_addRootPath = settings.value(ADDROOTPATH_ENABLED, true).toBool();
     m_defaultHash = settings.value(DEFAULTHASH, "SHA256").toString();
     m_rootPathTyp = settings.value(ROOTPATHTYP, "dynamic").toString();
     m_format = settings.value(FORMAT, "default").toString();
 }

void Options::writeSettings()
{
    QSettings settings;

    settings.setValue(ADDROOTPATH_ENABLED, m_addRootPath);
    settings.setValue(DEFAULTHASH, m_defaultHash);
    settings.setValue(ROOTPATHTYP, m_rootPathTyp);
    settings.setValue(FORMAT, m_format);
}

bool Options::addRootPath() const
{
    return m_addRootPath;
}

void Options::setAddRootPath(bool addRootPath)
{
    m_addRootPath = addRootPath;
}

QString Options::defaultHash() const
{
    return m_defaultHash;
}

void Options::setDefaultHash(const QString &defaultHash)
{
    m_defaultHash = defaultHash;
}

QString Options::rootPathTyp() const
{
    return m_rootPathTyp;
}

void Options::setRootPathTyp(const QString &rootPathTyp)
{
    m_rootPathTyp = rootPathTyp;
}

QString Options::format() const
{
    return m_format;
}

void Options::setFormat(const QString &format)
{
    m_format = format;
}

void Options::apply()
{
    emit initProperties();
    emit init_hashtyp();
}




