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

#ifndef FILTERSETTINGS_H
#define FILTERSETTINGS_H

#include <QDialog>
#include <QTableWidget>
#include <QMessageBox>
#include <QInputDialog>
#include "xmlconfigio.h"
#include "globaldefs.h"


namespace Ui {
    class FilterSettings;
}

class FilterSettings : public QDialog
{
    Q_OBJECT

public:
    explicit FilterSettings(QWidget *parent = 0);
    ~FilterSettings();

public slots:
        void comboIndexChanged(QString);
        void pushButtonNewFilterClicked();
        void pushButtonRemoveFilterClicked();
        void pushButtonAddInclude();
        void pushButtonDelInclude();
        void pushButtonAddExclude();
        void pushButtonDelExclude();
        void pushButtonSaveClicked();
        void inItemChanged(QTableWidgetItem*);
        void exItemChanged(QTableWidgetItem*);

private:
    void init(void);
    void printIncludeFilter(QString filterName);
    void printExcludeFilter(QString filterName);
    Ui::FilterSettings *ui;
    XMLConfigIO cf;
    QMultiMap<QString, QString> m_includeFileFilter;
    QMultiMap<QString, QString> m_excludeFileFilter;
    QList<QString> m_key;
    QList<QTableWidgetItem *> m_tableIncludeFilter;
    QList<QTableWidgetItem *> m_tableExcludeFilter;
};

#endif // FILTERSETTINGS_H
