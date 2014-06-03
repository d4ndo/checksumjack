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

#ifndef GLOBALDEFS_H
#define GLOBALDEFS_H

#ifdef Q_OS_WIN
#define FILTER "/checksumJack/filter.xml"
#else
//#define FILTER "/.checksumJack/filter.xml"
#endif

#define BLOCKSIZE   (65536)

#define DESKTOP "desktop"
#define DOCUMENTS "documents"
#define APPLICATIONS "applications"
#define MUSIC "music"
#define MOVIE "movie"
#define PICTURE "picture"
#define TEMP "temp"
#define DATA "data"
#define DOWNLOAD "download"
#define HOME "home"

#define LOW     (0)
#define HIGH    (100)
#define COLOUR  (255)

#endif // GLOBALDEFS_H
