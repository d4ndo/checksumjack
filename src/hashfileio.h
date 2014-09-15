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

#ifndef HASHFILEIO_H
#define HASHFILEIO_H

#include <QFile>
#include <QTextStream>

class HashFileIO
{
protected:
    HashFileIO(const HashFileIO& hf);
    HashFileIO& operator=(const HashFileIO& hf);

public:
    HashFileIO(QString filename)
        : mfilename(filename),
          mformat("gnu"),
          mInputdata(mfilename),
          mOutputdata(mfilename),
          mIn(&mInputdata),
          mOut(&mOutputdata)
    {}

    ~HashFileIO(void);

    QString& getHashFileName(void);
    void setHashFileName(const QString& filename);
    bool openHashFileReading(void);
    bool openHashFileWriteing(void);
    void closeHashFile(void);
    void writerToHashFile(const QString& hashStr, const QString& rootpath, const  QString& fileName, const QString& hashtyp);
    QString readFromHashFile(void);
    QString getformat() const;
    void setformat(const QString &value);
    bool getfullPath() const;
    void setfullPath(bool value);

private:
    QString mfilename;
    QString mformat;
    bool mfullPath;
    QFile mInputdata;
    QFile mOutputdata;
    QTextStream mIn;
    QTextStream mOut;
};

#endif // HASHFILEIO_H
