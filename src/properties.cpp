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

Properties::Properties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Properties)
{
    ui->setupUi(this);
    cf.readFile(QDir::home().absolutePath() + FILTER);
    m_addRootPath = cf.isAddRootPath();
    m_rootPathTyp = cf.getRootPathTyp();
    m_style = cf.getStyle();
    m_hashtyp = cf.getDefaultHash();

    if (m_addRootPath)
    {
        ui->radioYes->setChecked(true);
    } else  {
        ui->radioNo->setChecked(true);
    }

    if (m_rootPathTyp.contains("dynamic"))
    {
        ui->comboBoxRootPath->setCurrentIndex(0);
    }
    if (m_rootPathTyp.contains("static"))
    {
        ui->comboBoxRootPath->setCurrentIndex(1);
    }

    if (m_style.contains("MD5"))
    {
        ui->radioMD5->setChecked(true);
    }

    if (m_style.contains("BSD"))
    {
        ui->radioBSD->setChecked(true);
    }

    /* hashtyp */
    if (m_hashtyp.contains("MD5"))
    {
        ui->comboBoxHashTyp->setCurrentIndex(0);
    }
    if (m_hashtyp.contains("SHA-1"))
    {
        ui->comboBoxHashTyp->setCurrentIndex(1);
    }
    if (m_hashtyp.contains("RIPEMD-160"))
    {
        ui->comboBoxHashTyp->setCurrentIndex(2);
    }
    if (m_hashtyp.contains("SHA224"))
    {
        ui->comboBoxHashTyp->setCurrentIndex(3);
    }
    if (m_hashtyp.contains("SHA256"))
    {
        ui->comboBoxHashTyp->setCurrentIndex(4);
    }
    if (m_hashtyp.contains("SHA384"))
    {
        ui->comboBoxHashTyp->setCurrentIndex(5);
    }
    if (m_hashtyp.contains("SHA512"))
    {
        ui->comboBoxHashTyp->setCurrentIndex(6);
    }
    if (m_hashtyp.contains("WHIRLPOOL"))
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
    cf.setDefaultHash(m_hashtyp);
    cf.setStyle(m_style);
    cf.setRootPathTyp(m_rootPathTyp);
    cf.AddRootPath(m_addRootPath);
    cf.writeToFile(QDir::home().absolutePath() + FILTER);
}

void Properties::on_radioYes_clicked()
{
    ui->comboBoxRootPath->setDisabled(false);
    m_addRootPath = true;
}

void Properties::on_radioNo_clicked()
{
    ui->comboBoxRootPath->setDisabled(true);
    m_addRootPath = false;
}

void Properties::on_radioMD5_clicked()
{
    m_style = "MD5";
}

void Properties::on_radioBSD_clicked()
{
    m_style = "BSD";
}

void Properties::on_comboBoxRootPath_activated(int index)
{
    if (index == 0)
    {
        m_rootPathTyp = "dynamic";
    }
    if (index == 1)
    {
        m_rootPathTyp = "static";
    }
}

void Properties::on_comboBoxHashTyp_activated(int index)
{
    if (index == 0)
    {
        m_hashtyp = "MD5";
    }
    if (index == 1)
    {
        m_hashtyp = "SHA-1";
    }
    if (index == 2)
    {
        m_hashtyp = "RIPEMD-160";
    }
    if (index == 3)
    {
        m_hashtyp = "SHA224";
    }
    if (index == 4)
    {
        m_hashtyp = "SHA256";
    }
    if (index == 5)
    {
        m_hashtyp = "SHA384";
    }
    if (index == 6)
    {
        m_hashtyp = "SHA512";
    }
    if (index == 7)
    {
        m_hashtyp = "WHIRLPOOL";
    }
}
