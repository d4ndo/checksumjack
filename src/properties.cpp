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

#include "properties.h"
#include "ui_properties.h"
#include "globaldefs.h"

Properties::Properties(Options *opt, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Properties),
    options(opt)
{
    ui->setupUi(this);
    options->readSettings();

    if (options->addRootPath())
    {
        ui->radioYes->setChecked(true);
    } else  {
        ui->radioNo->setChecked(true);
    }

    if (options->rootPathTyp().contains("dynamic"))
    {
        ui->comboBoxRootPath->setCurrentIndex(0);
    }
    if (options->rootPathTyp().contains("static"))
    {
        ui->comboBoxRootPath->setCurrentIndex(1);
    }

    if (options->style().contains("MD5"))
    {
        ui->radioMD5->setChecked(true);
    }

    if (options->style().contains("BSD"))
    {
        ui->radioBSD->setChecked(true);
    }

    /* hashtyp */
   if (options->defaultHash().contains("MD5"))
   {
   ui->comboBoxHashTyp->setCurrentIndex(0);
   }
   if (options->defaultHash().contains("SHA-1"))
   {
   ui->comboBoxHashTyp->setCurrentIndex(1);
   }
   if (options->defaultHash().contains("RIPEMD-160"))
   {
   ui->comboBoxHashTyp->setCurrentIndex(2);
   }
   if (options->defaultHash().contains("SHA224"))
   {
   ui->comboBoxHashTyp->setCurrentIndex(3);
   }
   if (options->defaultHash().contains("SHA256"))
   {
   ui->comboBoxHashTyp->setCurrentIndex(4);
   }
   if (options->defaultHash().contains("SHA384"))
   {
   ui->comboBoxHashTyp->setCurrentIndex(5);
   }
   if (options->defaultHash().contains("SHA512"))
   {
   ui->comboBoxHashTyp->setCurrentIndex(6);
   }
   if (options->defaultHash().contains("WHIRLPOOL"))
   {
   ui->comboBoxHashTyp->setCurrentIndex(7);
   }
}

Properties::~Properties()
{
    delete ui;
}

/* Save clicked */
void Properties::on_buttonBox_accepted()
{
    options->writeSettings();
    options->apply();
}

void Properties::on_radioYes_clicked()
{
    ui->comboBoxRootPath->setDisabled(false);
    options->setAddRootPath(true);
}

void Properties::on_radioNo_clicked()
{
    ui->comboBoxRootPath->setDisabled(true);
    options->setAddRootPath(false);
}

void Properties::on_radioMD5_clicked()
{
    options->setStyle("MD5");
}

void Properties::on_radioBSD_clicked()
{
    options->setStyle("BSD");
}

void Properties::on_comboBoxRootPath_activated(int index)
{
    if (index == 0)
    {
        options->setAddRootPath("dynamic");
    }
    if (index == 1)
    {
        options->setAddRootPath("static");
    }
}

void Properties::on_comboBoxHashTyp_activated(int index)
{
    if (index == 0)
    {
        options->setDefaultHash("MD5");
    }
    if (index == 1)
    {
        options->setDefaultHash("SHA-1");
    }
    if (index == 2)
    {
        options->setDefaultHash("RIPEMD-160");
    }
    if (index == 3)
    {
        options->setDefaultHash("SHA224");
    }
    if (index == 4)
    {
        options->setDefaultHash("SHA256");
    }
    if (index == 5)
    {
        options->setDefaultHash("SHA384");
    }
    if (index == 6)
    {
        options->setDefaultHash("SHA512");
    }
    if (index == 7)
    {
        options->setDefaultHash("WHIRLPOOL");
    }
}
