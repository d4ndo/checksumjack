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

#include "hashfileio.h"

HashFileIO::~HashFileIO()
{}

QString& HashFileIO::getHashFileName(void)
{
    return mfilename;
}

void HashFileIO::setHashFileName(const QString& filename)
{
    mfilename = filename;
    mInputdata.setFileName(mfilename);
    mOutputdata.setFileName(mfilename);
}

bool HashFileIO::openHashFileReading(void)
{
    if (mInputdata.open(QFile::ReadOnly | QIODevice::Text))
    {
        return true;
    }
    return false;
}

bool HashFileIO::openHashFileWriteing(void)
{
    if (mOutputdata.open(QFile::WriteOnly))
    {
        return true;
    }
    return false;
}

void HashFileIO::closeHashFile(void)
{
    mInputdata.close();
    mOutputdata.close();
}

void HashFileIO::writerToHashFile(const QString &hashStr, const QString &rootpath, const QString &fileName, const QString &hashtyp)
{
    QString filepath;

    if (mfullPath == true)
    {
        filepath.append(rootpath);
        filepath.append(fileName);
    } else {
        filepath.append(fileName);
    }

    if (mformat.contains("gnu")) {
        mOut << hashStr << " " << filepath << endl;
    }
    if (mformat.contains("bsd")) {
        mOut << hashtyp << " (" << filepath << ")" << " = " << hashStr << endl;
    }
    if (mformat.contains("csv")) {
        mOut << hashStr << "," << filepath << endl;
    }
}

QString HashFileIO::readFromHashFile(void)
{
    QString line;

    if (mIn.atEnd())
    {
        return "EOF";
    } else {
        line = mIn.readLine();
        return line;
    }
}
QString HashFileIO::getformat() const
{
    return mformat;
}

void HashFileIO::setformat(const QString &value)
{
    mformat = value;
}

bool HashFileIO::getfullPath() const
{
    return mfullPath;
}

void HashFileIO::setfullPath(bool value)
{
    mfullPath = value;
}


