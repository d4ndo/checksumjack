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

#include <QtWidgets>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QtCore>
#include "mainwindow.h"

extern void callOpenCheckSumFile(MainWindow *dialog);
extern void callOpenFile(MainWindow *dialog, QString &argument);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QRegExp patternMD5ending("[Mm][Dd]5$");
    QRegExp patternSHA1ending("[Ss][Hh][Aa]1$");
    QRegExp patternRIPEMD160ending("[Rr][Mm][Dd]160$");
    QRegExp patternSHA2_224ending("[Ss][Hh][Aa]224$");
    QRegExp patternSHA2_256ending("[Ss][Hh][Aa]256$");
    QRegExp patternSHA2_384ending("[Ss][Hh][Aa]384$");
    QRegExp patternSHA2_512ending("[Ss][Hh][Aa]512$");
    QRegExp patternWHIRLPOOLending("[Ww][Hh][Ii][Rr][Ll][Pp][Oo][Oo][Ll]$");
    patternMD5ending.setPatternSyntax(QRegExp::RegExp2);
    patternSHA1ending.setPatternSyntax(QRegExp::RegExp2);
    patternRIPEMD160ending.setPatternSyntax(QRegExp::RegExp2);
    patternSHA2_224ending.setPatternSyntax(QRegExp::RegExp2);
    patternSHA2_256ending.setPatternSyntax(QRegExp::RegExp2);
    patternSHA2_384ending.setPatternSyntax(QRegExp::RegExp2);
    patternSHA2_512ending.setPatternSyntax(QRegExp::RegExp2);
    patternWHIRLPOOLending.setPatternSyntax(QRegExp::RegExp2);

    QStringList arg = app.arguments();
    app.setApplicationName("CSJ");
    app.setOrganizationName("checksumjack");
    app.setOrganizationDomain("www.checksumjack.org");
    MainWindow dialog(arg);
    /*default */
    QString hash;

    /* verify checksumfile */
    if (arg.size() > 1)
    {
        if (arg.at(1).contains(patternMD5ending) ||
            arg.at(1).contains(patternSHA1ending)||
            arg.at(1).contains(patternRIPEMD160ending)||
            arg.at(1).contains(patternSHA2_224ending) ||
            arg.at(1).contains(patternSHA2_256ending) ||
            arg.at(1).contains(patternSHA2_384ending) ||
            arg.at(1).contains(patternSHA2_512ending) ||
            arg.at(1).contains(patternWHIRLPOOLending))
        {
            QFuture<void> future = QtConcurrent::run(callOpenCheckSumFile, &dialog);
            dialog.show();
            future.waitForFinished();
        } else {
            /* to do: increase getopt for hash type */
            hash = "none";
            QFuture<void> future = QtConcurrent::run(callOpenFile, &dialog, hash);
            dialog.show();
            future.waitForFinished();
        }
    } else {
            dialog.show();
    }
    return app.exec();
}

extern void callOpenCheckSumFile(MainWindow *dialog)
{
    dialog->callActionOpenChecksumFile();
}

extern void callOpenFile(MainWindow *dialog, QString &argument)
{
    dialog->callActionOpenFile(argument);
}
