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

#define __MSVCRT_VERSION__  0x0601

#include "fileio.h"
#include "globaldefs.h"

using namespace std;

FileIO::~FileIO()
{}

QString& FileIO::getFileName(void)
{
    return mfilename;
}

void FileIO::setFileName(const QString& filename)
{
    mfilename = filename;
}

bool FileIO::openFileRead(void)
{
    mfin.open(QFile::encodeName(mfilename).data(), ios_base::in | ios_base::binary);
    if (!mfin.is_open())
    {
        return false;
    }
    /* get length of file in byte */
    mSize = 0;
      #ifdef Q_OS_WIN
        struct __stat64 st;
        if(_stat64(QFile::encodeName(mfilename).data(), &st) == 0)
        mSize = st.st_size;
      #else
        struct stat64 st;
        if (stat64(mfilename.toStdString().c_str(), &st) == 0)
        mSize = st.st_size;
      #endif
    return true;
}

void FileIO::cosefile(void)
{
    mfin.close();
}

QByteArray& FileIO::parseAll()
{
    char *puffer = NULL;
    puffer = new char [BLOCKSIZE];
    mByte.clear();
    if (mfin.read(puffer, BLOCKSIZE))
    {
        mByte.append(puffer, BLOCKSIZE);
    } else {
        quint64 delta = mfin.gcount();
        mfin.seekg (delta, ios::cur);
        mfin.read(puffer, delta);
        mByte.append(puffer, delta);
    }
    delete [] puffer;
    return mByte;
}

long long FileIO::getFileSize(void) const
{
    return mSize;
}
