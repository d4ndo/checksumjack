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

#ifndef REPORTITEM_H
#define REPORTITEM_H

#include <QTableWidget>
#include <QDebug>


class ReportItem
{
public:
    ReportItem();
    ~ReportItem();
    void init(void);
    ReportItem(const ReportItem& r);
    ReportItem &operator=(const ReportItem &r);
    bool operator<(const ReportItem &r) const;
    QString *filePath(void) const;
    QTableWidgetItem* fileItem(void) const;
    QTableWidgetItem* sizeItem(void) const;
    QTableWidgetItem* statusItem(void) const;
    void clear(void);

private:
    QString *mfilePath;
    QTableWidgetItem *mfileItem;
    QTableWidgetItem *msizeItem;
    QTableWidgetItem *mstatusItem;

};

QDebug operator<<(QDebug stream, const ReportItem& r);

#endif // REPORTITEM_H
