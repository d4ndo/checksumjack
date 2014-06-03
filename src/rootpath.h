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

#ifndef ROOTPATH_H
#define ROOTPATH_H

#include <QString>
#include <QtGui>
#include <QtCore>
#include <QStandardPaths>
#include <QDebug>
#include "globaldefs.h"

QString encodeDynamicPath(QString rootpath);
QString decodeDynamicPath(QString filetag);

QString encodeStaticPath(QString rootpath);
QString decodeStaticPath(QString filetag);

#endif // ROOTPATH_H
