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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtCore>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenuBar>
#include <QProgressBar>
#include <QDesktopServices>
#include <QUrl>
#include "fileio.h"
#include "xmlconfigio.h"
#include "reportitem.h"
#include "about.h"
#include "properties.h"
#include "hashfileio.h"
#include "filtersettings.h"
#include "verifydialog.h"
#include "globaldefs.h"
#include "fileio.h"
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/ripemd.h>
#include <openssl/whrlpool.h>
#include <qdesktopservices.h>
#include "reportitem.h"

namespace Ui {
    class MainWindow;
}

class Options;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    MainWindow(QStringList& arg, QWidget *parent = 0);
    ~MainWindow();
    void callActionOpenChecksumFile();
    void callActionOpenFile(QString &argument);

protected:
    void changeEvent(QEvent *e);
    MainWindow& operator=(const MainWindow& mw);
    MainWindow(const MainWindow& mw);

private slots:
    void comboIndexChanged(QString);
    void checkBoxtoggle(bool);
    void tableWidget_DoubleClick(QTableWidgetItem *t);
    void on_actionOpen_file_triggered();
    void on_actionOpen_dir_triggered();
    void on_actionOpen_Hash_File_triggered();
    void on_actionSave_Hash_File_triggered();
    void on_actionAbout_triggered();
    void on_actionProperties_triggered();
    void on_actionFilter_triggered();
    void on_actionMD5_triggered();
    void on_actionSHA_1_triggered();
    void on_actionRIPEMD_160_triggered();
    void on_actionSHA_2_224_triggered();
    void on_actionSHA_2_256_triggered();
    void on_actionSHA_2_384_triggered();
    void on_actionSHA_2_512_triggered();
    void on_actionWhirlpool_triggered();
    void on_actionDonate_triggered();
    void on_actionRefresh_triggered();
    void on_actionClear_All_triggered();
    void on_tabWidget_currentChanged(int index);
    void on_actionStop_triggered();
    void on_actionQuit_triggered();
    void showContextMenu(const QPoint&);
    void showContextMenu2(const QPoint&);
    void verifyCheckSum_triggered();
    void openSelected_triggered();
    void open2Selected_triggered();
    void dropEvent(QDropEvent *de);
    void dragMoveEvent(QDragMoveEvent *de);
    void dragEnterEvent(QDragEnterEvent *event);
    void drag_checksumfiles(QStringList &files);
    void on_radioDynamic_clicked();
    void on_radioByTag_clicked();
    void on_radioStatic_clicked(bool checked);
    void on_radioNoRootPath_clicked();
    void on_toolButton_clicked();
    void initProperties();
    void init_hashtyp();

private:
    void init();
    void initInclude();
    void initFilterSettings(void);
    void scanDir(const QString &rootpath, const QString &path);
    void calculateCheckSum();
    void calculateValid();
    void printReport(void);
    void pre(void);
    void post(void);
    void removeNegativeFiles(QList<ReportItem> &report);
    void sortreport(void);
    void rootpath(void);
    void reInialise_ComboBox(void);

    Ui::MainWindow *ui;
    QStringList m_arg;
    QActionGroup *HashGroup;
    QString m_hashtyp;
    FileIO pf;
    XMLConfigIO cf;
    Options *options;

    QList<ReportItem> root;
    QList<ReportItem> report;
    QList<ReportItem> report2;

    QStringList mchecksumfiles;
    QStringList mhashes;
    QDir mdir;
    QDir mdir2;

    QMultiMap<QString, QString> m_includeFileFilter;
    QMultiMap<QString, QString> m_excludeFileFilter;
    QList<QString> m_key;
    QString m_all;
    QProgressBar *progressbar;
    QMovie *m_movie;
    bool mStop;
    bool mdragflag;
    bool mskip;
    bool mprogress;
    QString m_rootPathTyp;
    QString m_style;

    QMenu *checksumMenu;
    QAction *verifyCheckSum;
    QAction *openSelected;

    QMenu *checksumMenu2;
    QAction *open2Selected;

};

#endif // MAINWINDOW_H
