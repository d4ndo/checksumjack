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

#include "rootpath.h"

QString encodeDynamicPath(QString rootpath)
{
    QString dynamicpath = "none";
    QRegExp patternNo("^\\.{2}");
    QRegExp patternNope;
    patternNo.setPatternSyntax(QRegExp::RegExp2);
    patternNope.setPatternSyntax(QRegExp::RegExp2);

    QList<QDir> dirlist;

    QDir desktop(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).at(0));
    QDir documents(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0));
    QDir applications(QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).at(0));
    QDir music(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0));
    QDir movie(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).at(0));
    QDir picture(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).at(0));
    QDir temp(QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0));
    QDir data(QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0));
    QDir download(QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).at(0));
    QDir home(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).at(0));

    dirlist.append(desktop);
    dirlist.append(documents);
    dirlist.append(applications);
    dirlist.append(music);
    dirlist.append(movie);
    dirlist.append(picture);
    dirlist.append(temp);
    dirlist.append(data);
    dirlist.append(download);
    dirlist.append(home);

    for (int i = 0; i < dirlist.size(); i++) {
        QString d;
        if(i == 0) { d = DESKTOP; }
        if(i == 1) { d = DOCUMENTS; }
        if(i == 2) { d = APPLICATIONS; }
        if(i == 3) { d = MUSIC; }
        if(i == 4) { d = MOVIE; }
        if(i == 5) { d = PICTURE; }
        if(i == 6) { d = TEMP; }
        if(i == 7) { d = DATA; }
        if(i == 8) { d = DOWNLOAD; }
        if(i == 9) { d = HOME; }

        if (rootpath.contains(dirlist.at(i).absolutePath()))
        {
            QString temp = dirlist.at(i).relativeFilePath(rootpath);
            if (temp.isEmpty())
            {
                dynamicpath = "[";
                dynamicpath.append(d);
                dynamicpath.append(temp);
                dynamicpath.append("]");
                return dynamicpath;
            } else {
                dynamicpath = "[";
                dynamicpath.append(d);
                dynamicpath.append(",");
                dynamicpath.append(temp.replace("/",","));
                dynamicpath.append(",");
                dynamicpath.append("]");
                return dynamicpath;
            }
        }
    }
    return "none";
}


QString decodeDynamicPath(QString filetag)
{
    QDir desktop(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).at(0));
    QDir documents(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0));
    QDir applications(QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).at(0));
    QDir music(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).at(0));
    QDir movie(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).at(0));
    QDir picture(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).at(0));
    QDir temp(QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0));
    QDir data(QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0));
    QDir download(QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).at(0));
    QDir home(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).at(0));

    if (filetag.contains(DESKTOP))
    {
        QString tmp = filetag.replace(DESKTOP, desktop.absolutePath());
        QString ret = tmp.replace(",","/");
        return ret;
    }
    if (filetag.contains(DOCUMENTS))
    {
        QString tmp = filetag.replace(DOCUMENTS, documents.absolutePath());
        QString ret = tmp.replace(",","/");
        return ret;
    }
    if (filetag.contains(APPLICATIONS))
    {
        QString tmp = filetag.replace(APPLICATIONS, applications.absolutePath());
        QString ret = tmp.replace(",","/");
        return ret;
    }
    if (filetag.contains(MUSIC))
    {
        QString tmp = filetag.replace(MUSIC, music.absolutePath());
        QString ret = tmp.replace(",","/");
        return ret;
    }
    if (filetag.contains(MOVIE))
    {
        QString tmp = filetag.replace(MOVIE, movie.absolutePath());
        QString ret = tmp.replace(",","/");
        return ret;
    }
    if (filetag.contains(PICTURE))
    {
        QString tmp = filetag.replace(PICTURE, picture.absolutePath());
        QString ret = tmp.replace(",","/");
        return ret;
    }
    if (filetag.contains(TEMP))
    {
        QString tmp = filetag.replace(TEMP, temp.absolutePath());
        QString ret = tmp.replace(",","/");
        return ret;
    }
    if (filetag.contains(DATA))
    {
        QString tmp = filetag.replace(DATA, data.absolutePath());
        QString ret = tmp.replace(",","/");
        return ret;
    }
    if (filetag.contains(DOWNLOAD))
    {
        QString tmp = filetag.replace(DOWNLOAD, download.absolutePath());
        QString ret = tmp.replace(",","/");
        return ret;
    }
    if (filetag.contains(HOME))
    {
        QString tmp = filetag.replace(HOME, home.absolutePath());
        QString ret = tmp.replace(",","/");
        return ret;
    }

    return "none";
}


QString encodeStaticPath(QString rootpath)
{
    QString ret;
    QString temp;
    ret.append("[");
    temp.append(rootpath.replace(":",";"));
    ret.append(temp.replace("/",","));
    ret.append("]");
    return ret;
}

QString decodeStaticPath(QString filetag)
{
    QString ret;
    QString temp;
    temp = filetag.replace(",","/");
    ret = temp.replace(";",":");
    return ret;
}
