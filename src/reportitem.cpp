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

#include "reportitem.h"

ReportItem::ReportItem()
{
    init();
}

ReportItem::~ReportItem()
{
}

void ReportItem::init()
{
    mfilePath = new QString("");
    mfileItem = new QTableWidgetItem("");
    mfileItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    mfileItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    msizeItem = new QTableWidgetItem("");
    msizeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    msizeItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    mstatusItem = new QTableWidgetItem("");
    mstatusItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    mstatusItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
}

ReportItem::ReportItem(const ReportItem &r)
{
    mfilePath = r.filePath();
    mfileItem = r.fileItem();
    msizeItem = r.sizeItem();
    mstatusItem = r.statusItem();
}

ReportItem &ReportItem::operator =(const ReportItem &r)
{
    if(this != &r)
    {
        mfilePath = r.filePath();
        mfileItem = r.fileItem();
        msizeItem = r.sizeItem();
        mstatusItem = r.statusItem();
    }
    return *this;
}

bool ReportItem::operator <(const ReportItem &r) const
{
    if (mfileItem->text().toLower() < r.fileItem()->text().toLower())
    {
        return true;
    } else {
        return false;
    }
}

QString *ReportItem::filePath() const
{
    return mfilePath;
}

QTableWidgetItem *ReportItem::fileItem() const
{
    return mfileItem;
}

QTableWidgetItem *ReportItem::sizeItem() const
{
    return msizeItem;
}

QTableWidgetItem *ReportItem::statusItem() const
{
    return mstatusItem;
}

void ReportItem::clear()
{
    mfilePath->clear();
    mfileItem->setText("");
    msizeItem->setText("");
    mstatusItem->setText("");
}


QDebug operator<<(QDebug stream, const ReportItem &r)
{

    stream.nospace() << "(" << QString(r.filePath()->data()) << ", "
                            << r.fileItem()->text()  << ", "
                            << r.sizeItem()->text() << ", "
                            << r.statusItem()->text()  << ", "  << ")";
    return stream.space();
}
