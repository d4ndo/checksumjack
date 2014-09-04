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

#ifndef XMLCONFIGIO_H
#define XMLCONFIGIO_H

#include <QtXml>
#include <QMultiMap>
#include <QMessageBox>

class XMLConfigIO
{
protected:
    XMLConfigIO(const XMLConfigIO& obj);
    XMLConfigIO& operator=(const XMLConfigIO& obj);

public:
    XMLConfigIO();
    ~XMLConfigIO();
    bool readFile(QString filename);
    bool writeToFile(QString filename);

    QMultiMap<QString, QString>& getIncludeFileFilter(void);
    void setIncludeFileFilter(const QMultiMap<QString, QString>&);
    QMultiMap<QString, QString>& getExcludeFileFilter(void);
    void setExcludeFileFilter(const QMultiMap<QString, QString>&);
    QList<QString>& getKey(void);
    void setKey(const QList<QString>&);

private:
    void readAll(void);
    void readInclude(QString name);
    void readExclude(QString name);
    void writeFilter(int);
    void init();

    QFile mfile;
    QMultiMap<QString, QString> m_includeFileFilter;
    QMultiMap<QString, QString> m_excludeFileFilter;
    QList<QString> m_filter;
    QXmlStreamReader reader;
    QXmlStreamWriter writer;
};

#endif // XMLCONFIGIO_H
