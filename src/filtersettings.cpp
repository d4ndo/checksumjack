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

#include "filtersettings.h"
#include "ui_filtersettings.h"

FilterSettings::FilterSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterSettings)
{
    ui->setupUi(this);
    cf.readFile(QDir::home().absolutePath() + FILTER);
    m_includeFileFilter = cf.getIncludeFileFilter();
    m_excludeFileFilter = cf.getExcludeFileFilter();
    m_key = cf.getKey();   
    init();
}

FilterSettings::~FilterSettings()
{
    delete ui;
}

void FilterSettings::init()
{
    QListIterator<QString> i(m_key);

    /* Initialise filter combobox */
    while (i.hasNext())
    {
       QString filterkey = i.next();
       ui->comboBox->addItem(filterkey);
    }
    ui->buttonBox->button(QDialogButtonBox::Save)->setText(tr("Save"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
}

void FilterSettings::printIncludeFilter(QString filterName)
{
    ui->tableWidget->blockSignals(true);
    QList<QString> keyList = m_includeFileFilter.values(filterName);
    ui->tableWidget->setRowCount(keyList.size());
    m_tableIncludeFilter.clear();

    for (int j =  0; j < keyList.size(); j++)
    {
        m_tableIncludeFilter.append(new QTableWidgetItem(keyList.at(j)));
        m_tableIncludeFilter.at(j)->setTextAlignment(Qt::AlignLeft);
        ui->tableWidget->setItem(j,0, m_tableIncludeFilter.at(j));
    }
    ui->tableWidget->clearSpans();
    ui->tableWidget->blockSignals(false);
}

/* Helper function to print exclude filter in table view Exclude Filter */
void FilterSettings::printExcludeFilter(QString filterName)
{
    ui->tableWidget->blockSignals(true);
    QList<QString> keyList = m_excludeFileFilter.values(filterName);
    ui->tableWidget_2->setRowCount(keyList.size());
    m_tableExcludeFilter.clear();
    for (int j =  0; j < keyList.size(); j++)
    {
        m_tableExcludeFilter.append(new QTableWidgetItem(keyList.at(j)));
        m_tableExcludeFilter.at(j)->setTextAlignment(Qt::AlignLeft);
        ui->tableWidget_2->setItem(j,0, m_tableExcludeFilter.at(j));
    }
    ui->tableWidget_2->clearSpans();
    ui->tableWidget->blockSignals(false);
}

void FilterSettings::comboIndexChanged(QString filterName)
{
    printIncludeFilter(filterName);
    printExcludeFilter(filterName);
}

void FilterSettings::pushButtonNewFilterClicked()
{
    bool ok;
    QString filterName = QInputDialog::getText(this,
                                                tr("Enter filter name"),
                                                tr("Enter filter name"),
                                                QLineEdit::Normal,
                                                tr("My filter"), &ok);
    if (ok && !filterName.isEmpty())
    {
        if(m_key.contains(filterName))
        {
            QString message = tr("Filter name already exists.\n"
                                 "Please choose another name.");
            QMessageBox alertDuplikatFilterName;
            alertDuplikatFilterName.setText(message);
            alertDuplikatFilterName.setStandardButtons(QMessageBox::Ok);
            alertDuplikatFilterName.exec();
        } else {
            m_key.append(filterName);
            qSort(m_key);

            /* Reinitialise filter combobox */
            QListIterator<QString> i(m_key);
            ui->comboBox->clear();

            while (i.hasNext())
            {
                QString filterkey = i.next();
                ui->comboBox->addItem(filterkey);
            }
            ui->comboBox->setCurrentIndex(m_key.indexOf(filterName));
        }
    }
 }

void FilterSettings::pushButtonRemoveFilterClicked()
{
    QString message = tr("Delete ");
    message += ui->comboBox->currentText();
    message += " ?\n";
    int ret = 0;
    QMessageBox removeFilterMessage;
    removeFilterMessage.setText(message);
    removeFilterMessage.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    removeFilterMessage.setButtonText(QMessageBox::Ok,tr("Yes"));
    removeFilterMessage.setButtonText(QMessageBox::Cancel,tr("No"));
    ret = removeFilterMessage.exec();

    switch(ret)
    {
        case QMessageBox::Ok:
            /* Delete QString containg current text selected at combobox */
            m_key.removeAt(m_key.indexOf(ui->comboBox->currentText()));
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
    }

    /* Reinitialise filter combobox */
    qSort(m_key);
    QListIterator<QString> i(m_key);
    ui->comboBox->clear();

    while (i.hasNext())
    {
       QString filterkey = i.next();
       ui->comboBox->addItem(filterkey);
    }
}

void FilterSettings::pushButtonAddInclude()
{
    QList<QString> keyList = m_includeFileFilter.values(ui->comboBox->currentText());
    bool ok;

    QString wildCardName = QInputDialog::getText(this,
                                                tr("Enter wildcard"),
                                                tr("Enter wildcard"),
                                                QLineEdit::Normal,
                                                "", &ok);
    if (ok && !wildCardName.isEmpty())
    {
        if(keyList.contains(wildCardName))
        {
            QString message = tr("Wildcard already exists.\n");
            QMessageBox alertDuplikatWildCard;
            alertDuplikatWildCard.setText(message);
            alertDuplikatWildCard.setStandardButtons(QMessageBox::Ok);
            alertDuplikatWildCard.exec();
        } else {
            m_includeFileFilter.insert(ui->comboBox->currentText(), wildCardName);
            printIncludeFilter(ui->comboBox->currentText());
        }
    }
}

void FilterSettings::pushButtonDelInclude()
{
    QList<QString> keyList = m_includeFileFilter.values(ui->comboBox->currentText());
    for (int i = 0; i < ui->tableWidget->selectedItems().size(); i++)
    {
        m_includeFileFilter.remove(ui->comboBox->currentText(),
                                   keyList.at(ui->tableWidget->selectedItems().at(i)->row()));
    }
    printIncludeFilter(ui->comboBox->currentText());
}

void FilterSettings::pushButtonAddExclude()
{
    QList<QString> keyList = m_excludeFileFilter.values(ui->comboBox->currentText());
    bool ok;

    QString wildCardName = QInputDialog::getText(this,
                                                tr("Enter regulare expression"),
                                                tr("Enter regulare expression"),
                                                QLineEdit::Normal,
                                                "", &ok);
    if (ok && !wildCardName.isEmpty())
    {
        if(keyList.contains(wildCardName))
        {
            QString message = tr("Regulare expression already exists.\n");
            QMessageBox alertDuplikatWildCard;
            alertDuplikatWildCard.setText(message);
            alertDuplikatWildCard.setStandardButtons(QMessageBox::Ok);
            alertDuplikatWildCard.exec();
        } else {
            m_excludeFileFilter.insert(ui->comboBox->currentText(), wildCardName);
            printExcludeFilter(ui->comboBox->currentText());
        }
    }
}

void FilterSettings::pushButtonDelExclude()
{
    QList<QString> keyList = m_excludeFileFilter.values(ui->comboBox->currentText());
    for (int i = 0; i < ui->tableWidget_2->selectedItems().size(); i++)
    {
        m_excludeFileFilter.remove(ui->comboBox->currentText(),
                                   keyList.at(ui->tableWidget_2->selectedItems().at(i)->row()));
    }
    printExcludeFilter(ui->comboBox->currentText());
}

void FilterSettings::pushButtonSaveClicked()
{
    cf.setIncludeFileFilter(m_includeFileFilter);
    cf.setExcludeFileFilter(m_excludeFileFilter);
    cf.setKey(m_key);
    cf.writeToFile(QDir::home().absolutePath() + FILTER);
}

void FilterSettings::inItemChanged(QTableWidgetItem *item)
{
    QStringList in = m_includeFileFilter.values(ui->comboBox->currentText());
    QMultiMap<QString, QString>::iterator i = m_includeFileFilter.find(ui->comboBox->currentText(),in.at(item->row()));
    i.value() = item->text();
}

void FilterSettings::exItemChanged(QTableWidgetItem *item)
{
    QStringList ex = m_excludeFileFilter.values(ui->comboBox->currentText());
    QMultiMap<QString, QString>::iterator e = m_excludeFileFilter.find(ui->comboBox->currentText(), ex.at(item->row()));
    e.value() = item->text();
}
