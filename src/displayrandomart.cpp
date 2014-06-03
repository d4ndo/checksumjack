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

#include "displayrandomart.h"

DisplayRandomArt::DisplayRandomArt(QWidget *parent) :
    QWidget(parent)
{
}

void DisplayRandomArt::paint(QPainter &painter)
{
    painter.setClipRect(QRect(0, 0, 100, 100));
    painter.setPen(Qt::NoPen);
//    painter.fillRect(QRect(0, 0, 100, 100), Qt::white);
    QPolygon polygon;


    int i = 0;
    QString temp;
    foreach (QChar c, mchecksum)
    {
        if (i < 4)
        {
            i++;
            temp.append(c);
        } else {
        bool ok;
            uint num =  temp.toUInt(&ok, 16);
            qsrand(num);
            int x = qrand() % ((HIGH + 1) - LOW) + LOW;
            int y = qrand() % ((HIGH + 1) - LOW) + LOW;
            polygon << QPoint(x, y);
            temp.clear();
            i = 0;
        }
    }

    QColor c(qrand() % ((COLOUR + 1) - LOW) + LOW, qrand() % ((COLOUR + 1) - LOW) + LOW, qrand() % ((COLOUR + 1) - LOW) + LOW);
    QPolygonF p1(polygon);
    QPainterPath x;
    x.addPolygon(p1);

    painter.setBrush(c);
    painter.setOpacity(0.5);
    QPen mypen;
    mypen.setColor(QColor(0, 0, 0));
    mypen.setJoinStyle(Qt::RoundJoin);
    mypen.setWidth(1);
    painter.setPen(mypen);

    painter.drawPath(x);
}


void DisplayRandomArt::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(painter);
    painter.end();
}

void DisplayRandomArt::setChecksumArt(QString checksum)
{
    this->mchecksum = checksum;
    update();
}
