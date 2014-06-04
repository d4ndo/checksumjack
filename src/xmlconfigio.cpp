/*
 * checksumJack is a multi platform hash calculating software.
 * Copyright (C) 2014 www.checksumJack.org
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "xmlconfigio.h"

XMLConfigIO::XMLConfigIO()
{
    init();
}

XMLConfigIO::~XMLConfigIO()
{}

void XMLConfigIO::init(void)
{
    mfile.setFileName("test.txt");
}

bool XMLConfigIO::readFile(QString filename)
{
    m_includeFileFilter.clear();
    m_excludeFileFilter.clear();
    m_filter.clear();
    mfile.setFileName(filename);
    /* If we can't open it, let's show an error message. */
    if (!mfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msgBox;
        msgBox.setText("Error. Can not open config file.");
        msgBox.exec();
        return false;
     }

    reader.setDevice(&mfile);
    reader.readNext();
    while(!reader.atEnd())
    {
        if (reader.isStartElement()) {
            if (reader.name() == "csj")
            {
                readAll();
            } else {
                reader.raiseError(QObject::tr("Not a CSJ config file"));
            }
        } else {
             reader.readNext();
        }
    }
    mfile.close();
    if(reader.hasError())
    {
        QMessageBox msgBox;
        msgBox.setText("Error. Can not read config file.");
        msgBox.exec();
        return false;
    } else if (mfile.error() != QFile::NoError) {
            QMessageBox msgBox;
            msgBox.setText("Error. No file.");
            msgBox.exec();
            return false;
    }
    return true;
}

bool XMLConfigIO::writeToFile(QString filename)
{
    mfile.setFileName(filename);
    /* If we can't open it, let's show an error message. */
    if (!mfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox msgBox;
        msgBox.setText("Error. Can not open config file for writing.");
        msgBox.exec();
        return false;
     }

    writer.setDevice(&mfile);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("csj");

    writeDefaultHash();
    writeHashFileProperties();

    for (int i = 0; i < m_filter.size(); ++i)
    {
        writeFilter(i);
    }
    writer.writeEndDocument();
    mfile.close();
    if(mfile.error())
    {
        QMessageBox msgBox;
        msgBox.setText("Error. Can not close config file.");
        msgBox.exec();
        return false;
    }
    return true;
}

QMultiMap<QString, QString>& XMLConfigIO::getIncludeFileFilter(void)
{
    return m_includeFileFilter;
}

void XMLConfigIO::setIncludeFileFilter(const QMultiMap<QString, QString>& includeFileFilter)
{
    m_includeFileFilter.clear();
    m_includeFileFilter = includeFileFilter;
}

QMultiMap<QString, QString> &XMLConfigIO::getExcludeFileFilter(void)
{
    return m_excludeFileFilter;
}

void XMLConfigIO::setExcludeFileFilter(const QMultiMap<QString, QString>& excludeFileFilter)
{
    m_excludeFileFilter.clear();
    m_excludeFileFilter = excludeFileFilter;
}

QList<QString> & XMLConfigIO::getKey(void)
{
    qSort(m_filter);
    return m_filter;
}

void XMLConfigIO::setKey(const QList<QString>& filter)
{
    m_filter.clear();
    m_filter = filter;
}

QString &XMLConfigIO::getDefaultHash()
{
    return m_defaultHash;
}

void XMLConfigIO::setDefaultHash(QString &defaultHash)
{
    m_defaultHash = defaultHash;
}

bool XMLConfigIO::isAddRootPath()
{
    return m_addRootPath;
}

void XMLConfigIO::AddRootPath(bool addRootPath)
{
    m_addRootPath = addRootPath;
}

QString &XMLConfigIO::getRootPathTyp()
{
    return m_rootPathTyp;
}

void XMLConfigIO::setRootPathTyp(QString &rootPathTyp)
{
    m_rootPathTyp = rootPathTyp;
}

QString XMLConfigIO::getStyle()
{
    return m_style;
}

void XMLConfigIO::setStyle(QString &style)
{
    m_style = style;
}

void XMLConfigIO::readAll(void)
{
    QStringRef name;

    while (!reader.atEnd()) {

        if ((reader.name() == "defaulthash") &&
            (reader.tokenType() == QXmlStreamReader::StartElement))
        {
            readDefaultHash();
        }
        if ((reader.name() == "hashfileproperties") &&
            (reader.tokenType() == QXmlStreamReader::StartElement))
        {
            readHashFileProperties();
        }
        if ((reader.name() == "filter") &&
            (reader.tokenType() == QXmlStreamReader::StartElement))
        {
            name = reader.attributes().value("name");
            m_filter.append(name.toString());
        }
        if ((reader.name() == "include") &&
            (reader.tokenType() == QXmlStreamReader::StartElement))
        {
            readInclude(name.toString());
        }
        if ((reader.name() == "exclude") &&
            (reader.tokenType() == QXmlStreamReader::StartElement))
        {
            readExclude(name.toString());
        }
        reader.readNext();
    }
}

void XMLConfigIO::readDefaultHash(void)
{
    m_defaultHash = reader.readElementText();
}

void XMLConfigIO::readHashFileProperties(void)
{
    reader.readNext();
    while(!(reader.tokenType() == QXmlStreamReader::EndElement &&
            reader.name() == "hashfileproperties")) {

        if(reader.tokenType() == QXmlStreamReader::StartElement) {
            if(reader.name() == "addrootpath") {
                if (reader.readElementText().contains("true"))
                {
                    m_addRootPath = true;
                } else {
                    m_addRootPath = false;
                }
            }
        }

        if(reader.tokenType() == QXmlStreamReader::StartElement) {
            if(reader.name() == "rootpathtyp") {
                m_rootPathTyp = reader.readElementText();
            }
        }

        if(reader.tokenType() == QXmlStreamReader::StartElement) {
            if(reader.name() == "style") {
                m_style = reader.readElementText();
            }
        }

        reader.readNext();
    }
}

void XMLConfigIO::readInclude(QString name)
{
    reader.readNext();

    while(!(reader.tokenType() == QXmlStreamReader::EndElement &&
            reader.name() == "include")) {

        if(reader.tokenType() == QXmlStreamReader::StartElement) {
            if(reader.name() == "f") {
                m_includeFileFilter.insert(name, reader.readElementText());
            }
        }
        reader.readNext();
    }
}

void XMLConfigIO::readExclude(QString name)
{
    reader.readNext();

    while(!(reader.tokenType() == QXmlStreamReader::EndElement &&
            reader.name() == "exclude")) {

        if(reader.tokenType() == QXmlStreamReader::StartElement) {
            if(reader.name() == "f") {
                m_excludeFileFilter.insert(name, reader.readElementText());
            }
        }
        reader.readNext();
    }
}

void XMLConfigIO::writeFilter(int i)
{
    QList<QString> tempInclude = m_includeFileFilter.values(m_filter.at(i));
    QList<QString> tempExclude = m_excludeFileFilter.values(m_filter.at(i));

    writer.writeStartElement("filter");
    writer.writeAttribute("name", m_filter.at(i));

    if (!tempInclude.isEmpty()) writer.writeStartElement("include");

    for (int i = 0; i < tempInclude.size(); ++i)
    {
        writer.writeTextElement("f", tempInclude.at(i));
    }
    if (!tempInclude.isEmpty()) writer.writeEndElement();
    if (!tempExclude.isEmpty()) writer.writeStartElement("exclude");
    for (int i = 0; i < tempExclude.size(); ++i)
    {
        writer.writeTextElement("f", tempExclude.at(i));
    }
    if (!tempExclude.isEmpty()) writer.writeEndElement();
    writer.writeEndElement();
}



void XMLConfigIO::writeDefaultHash(void)
{
    writer.writeTextElement("defaulthash", m_defaultHash);
}

void XMLConfigIO::writeHashFileProperties()
{
    writer.writeStartElement("hashfileproperties");

    if(m_addRootPath)
    {
        writer.writeTextElement("addrootpath", "true");
    } else {
        writer.writeTextElement("addrootpath", "false");
    }

    writer.writeTextElement("rootpathtyp", m_rootPathTyp);
    writer.writeTextElement("style", m_style);

    writer.writeEndElement();
}
