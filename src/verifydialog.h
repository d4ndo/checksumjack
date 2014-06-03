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

#ifndef VERIFYDIALOG_H
#define VERIFYDIALOG_H

#include <QDialog>

namespace Ui {
class VerifyDialog;
}

class VerifyDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit VerifyDialog(QWidget *parent = 0);
    ~VerifyDialog();
    void setCheckSum1(QString checksum1);

signals:
     void sigChecksumArt(QString);
    
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::VerifyDialog *ui;
};

#endif // VERIFYDIALOG_H
