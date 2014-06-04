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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "rootpath.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     m_arg.clear();
     init();
     init_hashtyp();
     initInclude();
}

MainWindow::MainWindow(QStringList& arg, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     m_arg.clear();
     for (int i = 1; i < arg.size(); i++)
     {
         m_arg.append(arg.at(i));
     }
     init();
     init_hashtyp();
     initInclude();
}

void MainWindow::init()
{
    cf.readFile(QDir::home().absolutePath() + FILTER);
    HashGroup = new QActionGroup(this);
    HashGroup->addAction(ui->actionMD5);
    HashGroup->addAction(ui->actionSHA_1);
    HashGroup->addAction(ui->actionRIPEMD_160);
    HashGroup->addAction(ui->actionSHA_2_224);
    HashGroup->addAction(ui->actionSHA_2_256);
    HashGroup->addAction(ui->actionSHA_2_384);
    HashGroup->addAction(ui->actionSHA_2_512);
    HashGroup->addAction(ui->actionWhirlpool);
    m_includeFileFilter = cf.getIncludeFileFilter();
    m_excludeFileFilter = cf.getExcludeFileFilter();
    m_key = cf.getKey();
    m_hashtyp = cf.getDefaultHash();
    m_addRootPath = cf.isAddRootPath();
    m_rootPathTyp = cf.getRootPathTyp();

    checksumMenu = new QMenu(this);
    verifyCheckSum = new QAction(this);
    openSelected = new QAction(this);

    checksumMenu2 = new QMenu(this);
    open2Selected = new QAction(this);

    connect(ui->tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(tableWidget_DoubleClick(QTableWidgetItem*)));
    connect(this->verifyCheckSum, SIGNAL(triggered()), this, SLOT(verifyCheckSum_triggered()));
    connect(this->openSelected, SIGNAL(triggered()), this, SLOT(openSelected_triggered()));
    connect(this->open2Selected, SIGNAL(triggered()), this, SLOT(open2Selected_triggered()));
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
    connect(ui->tableWidget_2, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu2(const QPoint&)));

    m_movie = new QMovie(":/new/prefix1/icon/loader2.gif");
    m_movie->setCacheMode(QMovie::CacheAll);
    ui->labelProgress->setMovie(m_movie);
    ui->toolButton->setEnabled(false);

    ui->tableWidget_2->setAcceptDrops(true);
    ui->tableWidget->setAcceptDrops(true);
    this->setAcceptDrops(true);
    mdragflag = false;
    mStop = false;
    progressbar = new QProgressBar(this);
    progressbar->setTextVisible(false);
    ui->checkBox->setHidden(true);
    ui->statusBar->addWidget(progressbar, 1);

    ui->comboBox->clear();
    QListIterator<QString> i(m_key);

    /* Initialise filter combobox */
    while (i.hasNext())
    {
       QString filterkey = i.next();
       ui->comboBox->addItem(filterkey);
    }
    mdir.entryList().clear();
    mchecksumfiles.clear();
    report.clear();
    root.clear();
}

void MainWindow::initInclude()
{
    QListIterator<QString> i(m_key);

    while (i.hasNext())
    {
        QString filterkey = i.next();
        QStringList filtercontent;


        m_all += filterkey;
        m_all += "(";

        filtercontent = m_includeFileFilter.values(filterkey);
        QListIterator<QString> j(filtercontent);

        while (j.hasNext())
        {
            m_all += j.next();
            m_all += " ";

        }

        m_all += ")";
        if (i.hasNext())
        {
            m_all += ";;";
        }
   }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


/*
 * Select files
 */
void MainWindow::on_actionOpen_file_triggered()
{
     QStringList files;
     files.clear();
     report.clear();
     root.clear();
     mdir.entryList().clear();
     ui->checkBox->setHidden(true);

     if (m_arg.isEmpty())
     {
         files = QFileDialog::getOpenFileNames(this, tr("Select files") , QDir::homePath(), m_all, 0);

     } else {
         files = m_arg;
         m_arg.clear();
     }

     if (!files.isEmpty()) {

        /* get root path of selected files */
        QFileInfo Info(files.at(0));
        QDir Infodir = Info.dir();

        for (int i = 0; i < files.size(); i++)
        {
             QFileInfo fileI(files.at(i));

             ReportItem rx;
             rx.fileItem()->setText(Infodir.relativeFilePath(fileI.absoluteFilePath()));
             rx.filePath()->append(Infodir.absolutePath());
             root.append(rx);
         }

         progressbar->reset();
         ui->commandLinkButtonClearAll->setVisible(true);
         ui->commandLinkButtonSaveChecksum->setVisible(true);
         ui->commandLinkButtonSaveChecksum->setDisabled(true);
         ui->actionSave_Hash_File->setDisabled(true);
         ui->tableWidget->horizontalHeaderItem(1)->setText(tr("Size"));

        sortreport();
        printReport();
        ui->actionRefresh->activate(QAction::Trigger);
     }
}


/*
 * Select Folder
 */
void MainWindow::on_actionOpen_dir_triggered()
{
    QStringList files;

    files.clear();
    ui->checkBox->setChecked(false);
    root.clear();
    report.clear();

    progressbar->reset();
    ui->checkBox->setHidden(false);
    ui->actionSave_Hash_File->setDisabled(true);

    ui->tableWidget->horizontalHeaderItem(1)->setText(tr("Size"));

    if (!mdragflag)
    {
        if (m_arg.isEmpty())
        {
            mdir = QFileDialog::getExistingDirectory(this, tr("Select directory") , QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        } else {
            QFileInfo info(m_arg.at(0));
            if (info.isSymLink())
            {
                mdir.setPath(info.symLinkTarget());
            } else {
                mdir.setPath(info.absoluteFilePath());
            }
           m_arg.clear();
        }
    } else {
        mdragflag = false;
    }

    mdir.absolutePath();
    ui->label_2->setText(mdir.canonicalPath());
    ui->groupBox->setTitle(tr("Report : "));
    ui->labelDir->setText(mdir.canonicalPath() + "/");

    QStringList filter;
    filter.clear();
    QFileInfoList filelist;
    filelist = mdir.entryInfoList(filter, QDir::Files, QDir::NoSort);

    for (int i = 0; i < filelist.size(); i++)
    {
        ReportItem r;
        r.fileItem()->setText(mdir.relativeFilePath(filelist.at(i).absoluteFilePath()));
        r.filePath()->append(mdir.absolutePath());
        root.append(r);
    }

    sortreport();
    printReport();
}


/*
 * Open Hash File triggered
 */
void MainWindow::on_actionOpen_Hash_File_triggered()
{
    ui->tabWidget->setCurrentIndex(1);
    ui->radioByTag->setChecked(true);

    if (m_arg.isEmpty())
    {
        QString checksumFilter = "Checksum ( *.sha1 *.md5 *.sha224 *.sha256 *.sha384 *.sha512 *.whirlpool )"
                                 ";; All ( * ) ;; SHA-1 ( *.sha1 ) ;; RIPEMD-160 ( *.ripemd160 ) ;; SHA224 ( *.sha224) ;; SHA256 ( *.sha256)"
                                 ";; SHA384 ( *.sha384 ) ;; SHA512 ( *.sha512 ) ;; MD5 ( *.md5 ) ;; WhirlPool ( *.whirlpool )";
        mchecksumfiles = QFileDialog::getOpenFileNames(this, tr("Select a checksum file"),
                                                      QDir::homePath(),
                                                       checksumFilter);
    } else {
        mchecksumfiles = m_arg;
        m_arg.clear();
    }

    if(!mchecksumfiles.isEmpty())
    {
        rootpath();
        calculateValid();
    }
}


/*
 * Save Hash File triggered
 */
void MainWindow::on_actionSave_Hash_File_triggered()
{
    QString fileEnding;
    fileEnding = "All ( * )";

    if (m_hashtyp == "MD5")
    {
        fileEnding = "MD5 ( *.md5 ) ;; All ( * )";
    } else if (m_hashtyp == "SHA-1") {
        fileEnding = " SHA-1 ( *.sha1 ) ;; All ( * )";
    } else if (m_hashtyp == "RIPEMD-160") {
        fileEnding = " RIPEMD-160 ( *.rmd160 ) ;; All ( * )";
    } else if (m_hashtyp == "SHA224") {
        fileEnding = "SHA224 (  *.sha224 ) ;; All ( * )";
    } else if (m_hashtyp == "SHA256") {
        fileEnding = "SHA256 ( *.sha256 ) ;; All ( * )";
    } else if (m_hashtyp == "SHA384") {
        fileEnding = "SHA384 ( *.sha384 ) ;; All ( * )";
    }  else if (m_hashtyp == "SHA512") {
        fileEnding = "SHA512 ( *.sha512 ) ;; All ( * )";
    } else if (m_hashtyp == "WHIRLPOOL") {
        fileEnding = " WhirlPool ( *.whirlpool ) ;; All ( * )";
    }

    if (!report.isEmpty())
    {
        QRegExp patternFile("^([^\\[]*)(\\[*)([^\\]]*)(\\]*)(\\.*)(.*)$");
        QRegExp patternMD5ending("\\.[Mm][Dd]5$");
        QRegExp patternSHA1ending("\\.[Ss][Hh][Aa]1$");
        QRegExp patternRIPEMD160ending("\\.[Rr][Mm][Dd]160$");
        QRegExp patternSHA2_224ending("\\.[Ss][Hh][Aa]224$");
        QRegExp patternSHA2_256ending("\\.[Ss][Hh][Aa]256$");
        QRegExp patternSHA2_384ending("\\.[Ss][Hh][Aa]384$");
        QRegExp patternSHA2_512ending("\\.[Ss][Hh][Aa]512$");
        QRegExp patternWHIRLPOOLending("\\.[Ww][Hh][Ii][Rr][Ll][Pp][Oo][Oo][Ll]$");
        patternFile.setPatternSyntax(QRegExp::RegExp2);
        patternMD5ending.setPatternSyntax(QRegExp::RegExp2);
        patternSHA1ending.setPatternSyntax(QRegExp::RegExp2);
        patternRIPEMD160ending.setPatternSyntax(QRegExp::RegExp2);
        patternSHA2_224ending.setPatternSyntax(QRegExp::RegExp2);
        patternSHA2_256ending.setPatternSyntax(QRegExp::RegExp2);
        patternSHA2_384ending.setPatternSyntax(QRegExp::RegExp2);
        patternSHA2_512ending.setPatternSyntax(QRegExp::RegExp2);
        patternWHIRLPOOLending.setPatternSyntax(QRegExp::RegExp2);

        QString fileN = QFileDialog::getSaveFileName(this, tr("Save File"), QString(report.at(0).filePath()->data()), fileEnding, NULL, QFileDialog::DontConfirmOverwrite);
        QString fileName;

        patternFile.indexIn(fileN);
        fileName = patternFile.cap(1);

        if(!fileName.isEmpty())
        {
            if (m_addRootPath)
            {
                QString temp;
                /* Add root path to file name dynamic */
                if(m_rootPathTyp.contains("dynamic"))
                {
                    temp = encodeDynamicPath(ui->labelDir->text());
                    if(temp.contains("none")) { m_rootPathTyp.clear(); m_rootPathTyp.append("static"); } else {
                        fileName.append(temp);
                    }
                }
                if (m_rootPathTyp.contains("static"))
                {
                    temp = encodeStaticPath(ui->labelDir->text());
                    fileName.append(temp);
                }
            }

            if (m_hashtyp == "MD5")
            {
                if (fileName.contains(patternMD5ending)) fileName.remove(patternMD5ending);
                fileName.append(".md5");
            } else if (m_hashtyp == "SHA-1")  {
                if (fileName.contains(patternSHA1ending)) fileName.remove(patternSHA1ending);
                fileName.append(".sha1");
            } else if (m_hashtyp == "RIPEMD-160")  {
                if (fileName.contains(patternRIPEMD160ending)) fileName.remove(patternRIPEMD160ending);
                fileName.append(".rmd160");
            } else if (m_hashtyp == "SHA224") {
                if (fileName.contains(patternSHA2_224ending)) fileName.remove(patternSHA2_224ending);
                fileName.append(".sha224");
            } else if (m_hashtyp == "SHA256") {
                if (fileName.contains(patternSHA2_256ending)) fileName.remove(patternSHA2_256ending);
                fileName.append(".sha256");
            } else if (m_hashtyp == "SHA384") {
                if (fileName.contains(patternSHA2_384ending)) fileName.remove(patternSHA2_384ending);
                fileName.append(".sha384");
            } else if (m_hashtyp == "SHA512") {
                if (fileName.contains(patternSHA2_384ending)) fileName.remove(patternSHA2_384ending);
                fileName.append(".sha512");
            } else if (m_hashtyp == "WHIRLPOOL") {
                if (fileName.contains(patternWHIRLPOOLending)) fileName.remove(patternWHIRLPOOLending);
                fileName.append(".whirlpool");
            }

            bool save = true;
            QFileInfo info(fileName);
            if (info.exists())
            {
                int ret = 0;
                QMessageBox fileexist;
                fileexist.setText(tr("File allready existing. Override File ?"));
                fileexist.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
                fileexist.setButtonText(QMessageBox::Cancel,tr("Cancel"));
                fileexist.setButtonText(QMessageBox::Yes,tr("Yes"));
                ret = fileexist.exec();
                switch(ret)
                {
                    case QMessageBox::Yes:
                        save = true;
                        break;
                    case QMessageBox::Cancel:
                        save = false;
                        break;
                    default:
                    break;
                }

            }
            if (save)
            {
                HashFileIO checksumfile(fileName);
                checksumfile.openHashFileWriteing();

                for (int i = 0; i < report.size(); i++)
                {
                    if (!report.isEmpty())
                    {
                        checksumfile.writerToHashFile(report.at(i).statusItem()->text(), report.at(i).fileItem()->text());
                    }
                }
                checksumfile.closeHashFile();
            }
        }
    }
}


void MainWindow::on_actionAbout_triggered()
{
    About aboutDia;
    aboutDia.setWindowTitle(tr("About checksumJack"));
    aboutDia.show();
    aboutDia.exec();
}

void MainWindow::comboIndexChanged(QString setfilter)
{
    ui->labelProgress->setMovie(m_movie);
    pre();
    m_movie->start();
    sortreport();
    printReport();
    m_movie->stop();
    post();
    mStop = false;
    ui->labelProgress->setText(" ");
}

void MainWindow::checkBoxtoggle(bool checked)
{
    ui->labelProgress->setMovie(m_movie);
    m_movie->start();
    ui->commandLinkButtonSaveChecksum->setDisabled(true);
    ui->actionSave_Hash_File->setDisabled(true);
    report.clear();
    root.clear();
    pre();

    if (checked == true)
    {

        scanDir(mdir.absolutePath(), mdir.absolutePath());
        if (mStop) { root.clear(); ui->checkBox->setChecked(false);}
    } else {
        QStringList filter;
        QFileInfoList filelist;
        filelist = mdir.entryInfoList(filter, QDir::Files, QDir::NoSort);

        for (int i = 0; i < filelist.size(); ++i)
        {
            ReportItem r;
            r.fileItem()->setText(mdir.relativeFilePath(filelist.at(i).absoluteFilePath()));
            r.filePath()->append(filelist.at(i).absolutePath());
            root.append(r);
        }
    }
    sortreport();
    printReport();
    m_movie->stop();
    post();
    mStop = false;
    ui->labelProgress->setText(" ");
}

/*
 * recursive helper function
 * get all files in subfolder
 * stored in member variable root
 */
void MainWindow::scanDir(const QString &rootpath, const QString &path)
{
    QDir dir(path);
    QDir file(path);
    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    file.setFilter(QDir::Files);
    QFileInfoList dirList = dir.entryInfoList();
    QFileInfoList fileList = file.entryInfoList();
    QDir rootdir(rootpath);
    qApp->processEvents();


    for (int l = 0; l < dirList.size(); l++)
    {
        if (mStop) { return; }
        scanDir(rootpath, dirList.at(l).absoluteFilePath());        
    }

    for (int i = 0; i < fileList.size(); i++)
    {
        if (mStop) { return; }
        qApp->processEvents();
        ReportItem r;
        r.fileItem()->setText(rootdir.relativeFilePath(fileList.at(i).absoluteFilePath()));
        r.filePath()->append(rootpath);
        root.append(r);
    }
}

void MainWindow::calculateCheckSum()
{
    mskip = false;
    progressbar->reset();
    if (report.size() != 1)
    {
        progressbar->setMaximum(report.size() -1);
    } else {
        progressbar->setMaximum(1);
    }
    ui->commandLinkButtonClearAll->setDisabled(true);
    ui->commandLinkButtonSaveChecksum->setDisabled(true);
    ui->actionSave_Hash_File->setDisabled(true);

    for (int i = 0; i < report.size(); ++i)
    {
        if(mStop) break;
        report.at(i).sizeItem()->setText("");
        report.at(i).statusItem()->setText("");
    }

    for (int j = 0; j < report.size(); ++j)
    {
        QByteArray line = "0";
        QString inProgress;
        QString mypath = QString(report.at(j).filePath()->data());

        pf.setFileName(mypath + "/" + report.at(j).fileItem()->text());
        ui->labelDir->setText(mypath + "/");

        inProgress = tr("File");
        inProgress.append(QString(" %1 ").arg(j + 1));
        inProgress.append("/");
        inProgress.append(QString(" %1 ").arg(report.size()));
        ui->label_2->setText(inProgress);
        ui->labelStatus->setText(tr("In Progress"));

        if (pf.openFileRead())
        {
            unsigned char hashbuf[64];

            MD5_CTX ctxMD5;
            MD5_Init(&ctxMD5);
            SHA_CTX ctxSHA;
            SHA1_Init(&ctxSHA);
            RIPEMD160_CTX ctxRIPEMD;
            RIPEMD160_Init(&ctxRIPEMD);
            SHA256_CTX ctxSHA2_224;
            SHA224_Init(&ctxSHA2_224);
            SHA256_CTX ctxSHA2_256;
            SHA256_Init(&ctxSHA2_256);
            SHA512_CTX ctxSHA2_384;
            SHA384_Init(&ctxSHA2_384);
            SHA512_CTX ctxSHA2_512;
            SHA512_Init(&ctxSHA2_512);
            WHIRLPOOL_CTX ctxWHIRLPOOL;
            WHIRLPOOL_Init(&ctxWHIRLPOOL);

            QByteArray outp;
            report.at(j).sizeItem()->setText(QString::number(pf.getFileSize()));
            progressbar->reset();
            progressbar->setMaximum(100);
            qApp->processEvents();
            quint64 c = 0;
            while (!(line = pf.parseAll()).isEmpty())
            {
                if(mStop) break;
                if (m_hashtyp == "MD5")
                {
                    MD5_Update(&ctxMD5, line, (size_t)line.size());
                }
                if (m_hashtyp == "SHA-1")
                {
                    SHA1_Update(&ctxSHA, line, (size_t)line.size());
                }
                if (m_hashtyp == "RIPEMD-160")
                {
                    RIPEMD160_Update(&ctxRIPEMD, line, (size_t)line.size());
                }
                if (m_hashtyp == "SHA224")
                {
                    SHA224_Update(&ctxSHA2_224, line, (size_t)line.size());
                }
                if (m_hashtyp == "SHA256")
                {
                    SHA256_Update(&ctxSHA2_256, line, (size_t)line.size());
                }
                if (m_hashtyp == "SHA384")
                {
                    SHA384_Update(&ctxSHA2_384, line, (size_t)line.size());
                }
                if (m_hashtyp == "SHA512")
                {
                    SHA512_Update(&ctxSHA2_512, line, (size_t)line.size());
                }
                if (m_hashtyp == "WHIRLPOOL")
                {
                    WHIRLPOOL_Update(&ctxWHIRLPOOL, line, (size_t)line.size());
                }

                c += line.size();
                progressbar->setValue((c * 100 / pf.getFileSize()));
                qApp->processEvents();
            }

            if(mStop)
            {
                ui->labelStatus->setText(tr("Aborted"));
                pf.cosefile();
                return;
            }
            if (m_hashtyp == "MD5")
            {
                MD5_Final(hashbuf, &ctxMD5);
                for (int i = 0; i < 16; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }
            if (m_hashtyp == "SHA-1")
            {
                SHA1_Final(hashbuf, &ctxSHA);
                for (int i = 0; i < 20; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }
            if (m_hashtyp == "RIPEMD-160")
            {
                RIPEMD160_Final(hashbuf, &ctxRIPEMD);
                for (int i = 0; i < 20; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }
            if (m_hashtyp == "SHA224")
            {
                SHA224_Final(hashbuf, &ctxSHA2_224);
                for (int i = 0; i < 28; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }
            if (m_hashtyp == "SHA256")
            {
                SHA256_Final(hashbuf, &ctxSHA2_256);
                for (int i = 0; i < 32; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }
            if (m_hashtyp == "SHA384")
            {
                SHA384_Final(hashbuf, &ctxSHA2_384);
                for (int i = 0; i < 48; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }
            if (m_hashtyp == "SHA512")
            {
                SHA512_Final(hashbuf, &ctxSHA2_512);
                for (int i = 0; i < 64; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }
            if (m_hashtyp == "WHIRLPOOL")
            {
                WHIRLPOOL_Final(hashbuf, &ctxWHIRLPOOL);
                for (int i = 0; i < 64; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }

            report.at(j).statusItem()->setText(outp.toHex());
            pf.cosefile();
        } else {
            if (!mskip)
            {
                int ret = 0;
                QMessageBox openError;
                openError.setText(tr("Could not open file."));
                openError.setStandardButtons(QMessageBox::Cancel | QMessageBox::YesToAll | QMessageBox::Yes);
                openError.setButtonText(QMessageBox::Cancel,tr("Cancel"));
                openError.setButtonText(QMessageBox::YesToAll,tr("Skip All"));
                openError.setButtonText(QMessageBox::Yes,tr("Skip"));
                ret = openError.exec();
                switch(ret)
                {
                    case QMessageBox::Yes:
                        pf.cosefile();
                        break;
                    case QMessageBox::YesToAll:
                        pf.cosefile();
                        mskip = true;
                        break;
                    case QMessageBox::Cancel:
                        pf.cosefile();
                        j = (report.size() - 1);
                        break;
                    default:
                    break;
                }
            }
        }
    }


    ui->labelStatus->setText(tr("Done"));
    ui->commandLinkButtonClearAll->setEnabled(true);
    if (!report.isEmpty()) {
        ui->commandLinkButtonSaveChecksum->setEnabled(true);
        ui->actionSave_Hash_File->setEnabled(true);
    } else {
        ui->commandLinkButtonSaveChecksum->setEnabled(false);
        ui->actionSave_Hash_File->setEnabled(false);
    }
}

void MainWindow::calculateValid()
{
    progressbar->reset();
    ui->actionSave_Hash_File->setDisabled(true);
    report2.clear();
    mhashes.clear();
    printReport();
    QRegExp patternMD5ending("[Mm][Dd]5$");    
    QRegExp patternSHA1ending("[Ss][Hh][Aa]1$");
    QRegExp patternRIPEMD160ending("[Rr][Mm][Dd]160$");
    QRegExp patternSHA2_224ending("[Ss][Hh][Aa]224$");
    QRegExp patternSHA2_256ending("[Ss][Hh][Aa]256$");
    QRegExp patternSHA2_384ending("[Ss][Hh][Aa]384$");
    QRegExp patternSHA2_512ending("[Ss][Hh][Aa]512$");
    QRegExp patternWHIRLPOOLending("[Ww][Hh][Ii][Rr][Ll][Pp][Oo][Oo][Ll]$");
    QRegExp patternComment("^\\s*[;|//].*$");
    QRegExp patternEmpty("^\\s*$");
    QRegExp patternMD5Style("^([^\\s]{32,128})(\\s+)(\\*{0,1})(.*)$");
    QRegExp patternBSDStyle("^([^\\s]*)(\\s*\\()([^\\)]*)(\\)\\s*)(=\\s*)(.{32,128})$");
    patternMD5Style.setPatternSyntax(QRegExp::RegExp2);
    patternBSDStyle.setPatternSyntax(QRegExp::RegExp2);
    patternMD5ending.setPatternSyntax(QRegExp::RegExp2);
    patternSHA1ending.setPatternSyntax(QRegExp::RegExp2);
    patternRIPEMD160ending.setPatternSyntax(QRegExp::RegExp2);
    patternSHA2_224ending.setPatternSyntax(QRegExp::RegExp2);
    patternSHA2_256ending.setPatternSyntax(QRegExp::RegExp2);
    patternSHA2_384ending.setPatternSyntax(QRegExp::RegExp2);
    patternSHA2_512ending.setPatternSyntax(QRegExp::RegExp2);
    patternWHIRLPOOLending.setPatternSyntax(QRegExp::RegExp2);
    patternComment.setPatternSyntax(QRegExp::RegExp2);
    patternEmpty.setPatternSyntax(QRegExp::RegExp2);
    QString ending = "";
    quint64 matches = 0;
    quint64 missmatches = 0;
    quint64 unreadable = 0;

    ui->label_match->setText(tr("Parity : "));
    ui->label_missmatch->setText(tr("Failure : "));
    ui->label_unreadable->setText(tr("Unreadable : "));
    ui->label_total_result->setText(tr("Total Result : "));

    ui->tableWidget_2->horizontalHeaderItem(1)->setText(tr("Hash Type"));
    ui->tableWidget_2->horizontalHeaderItem(2)->setText(tr("Status"));

    //ui->labelRootPath2->setText(mchecksumfiles.at(0));
    ui->groupBox_5->setTitle("Report : " + mchecksumfiles.at(0));

    if (mchecksumfiles.at(0).contains(patternMD5ending))
    {
        ending = "MD5";
    }
    if (mchecksumfiles.at(0).contains(patternSHA1ending))
    {
        ending = "SHA-1";
    }
    if (mchecksumfiles.at(0).contains(patternRIPEMD160ending))
    {
        ending = "RIPEMD-160";
    }
    if (mchecksumfiles.at(0).contains(patternSHA2_224ending))
    {
        ending = "SHA224";
    }
    if (mchecksumfiles.at(0).contains(patternSHA2_256ending))
    {
        ending = "SHA256";
    }
    if (mchecksumfiles.at(0).contains(patternSHA2_384ending))
    {
        ending = "SHA384";
    }
    if (mchecksumfiles.at(0).contains(patternSHA2_512ending))
    {
        ending = "SHA512";
    }
    if (mchecksumfiles.at(0).contains(patternWHIRLPOOLending))
    {
        ending = "WHIRLPOOL";
    }

    HashFileIO hashFile(mchecksumfiles.at(0));
    hashFile.openHashFileReading();
    QString line = hashFile.readFromHashFile();

    while (line != "EOF")
    {
        if (patternComment.indexIn(line) != 0 && patternEmpty.indexIn(line) != 0)
        {
            if (patternBSDStyle.indexIn(line) == 0)
            {
                /* BSD Style detected */
                ReportItem r;

                if (patternBSDStyle.cap(1) == "MD5")  r.sizeItem()->setText("MD5");
                if (patternBSDStyle.cap(1) == "SHA-1" || patternBSDStyle.cap(1) == "SHA1") r.sizeItem()->setText("SHA-1");
                if (patternBSDStyle.cap(1) == "RIPEMD-160" || patternBSDStyle.cap(1) == "RMD160") r.sizeItem()->setText("RIPEMD-160");
                if (patternBSDStyle.cap(1) == "SHA224") r.sizeItem()->setText("SHA224");
                if (patternBSDStyle.cap(1) == "SHA256") r.sizeItem()->setText("SHA256");
                if (patternBSDStyle.cap(1) == "SHA384") r.sizeItem()->setText("SHA384");
                if (patternBSDStyle.cap(1) == "SHA512") r.sizeItem()->setText("SHA512");
                if (patternBSDStyle.cap(1) == "WHIRLPOOL") r.sizeItem()->setText("WHIRLPOOL");

                mhashes.append(patternBSDStyle.cap(6));
                r.fileItem()->setText(QDir::fromNativeSeparators(patternBSDStyle.cap(3).trimmed()));
                r.filePath()->append(mdir2.absolutePath());
                report2.append(r);

            } else if (patternMD5Style.indexIn(line) == 0){
                /* MD5 style detected */
                mhashes.append(patternMD5Style.cap(1));
                ReportItem r;
                r.fileItem()->setText(QDir::fromNativeSeparators(patternMD5Style.cap(4).trimmed()));
                r.filePath()->append(mdir2.absolutePath());
                r.sizeItem()->setText(ending);
                report2.append(r);
            }
        }
        line = hashFile.readFromHashFile();
    }
    hashFile.closeHashFile();

    /* prepare and print table widget here */
    ui->tableWidget_2->setRowCount(report2.size());
    ui->tableWidget_2->setSortingEnabled(false);
    for (int j =  0; j < report2.size(); j++)
    {
        report2.at(j).statusItem()->setText("");

        ui->tableWidget_2->setItem(j,0, report2.at(j).fileItem());
        ui->tableWidget_2->setItem(j,1, report2.at(j).sizeItem());
        ui->tableWidget_2->setItem(j,2, report2.at(j).statusItem());
    }
    ui->tableWidget_2->resizeColumnToContents(0);
    ui->tableWidget_2->setSortingEnabled(true);

    /* compare here */
    if (report2.size() != 1)
    {
        progressbar->setMaximum(report2.size() -1);
    } else {
        progressbar->setMaximum(1);
    }
    ui->label_total_files->setText(tr("Total Files : ") + QString::number(report2.size()));
    for (int j = 0; j < report2.size(); ++j)
    {
        QByteArray line = "0";
        QString inProgress;
        QString comparefile;
        if (!ui->radioNoRootPath->isChecked())
        {
            comparefile.append(QString(report2.at(j).filePath()->data()));
            comparefile.append("/");
        }
        comparefile.append(report2.at(j).fileItem()->text());
        pf.setFileName(comparefile);
        inProgress = tr("File");
        inProgress.append(QString(" %1 ").arg(j + 1));
        inProgress.append("/");
        inProgress.append(QString(" %1 ").arg(report2.size()));
        ui->label_3->setText(inProgress);
        ui->labelStatus2->setText(tr("In Progress"));

        if (pf.openFileRead())
        {
            unsigned char hashbuf[64];

            MD5_CTX ctxMD5;
            MD5_Init(&ctxMD5);
            SHA_CTX ctxSHA;
            SHA1_Init(&ctxSHA);
            RIPEMD160_CTX ctxRIPEMD;
            RIPEMD160_Init(&ctxRIPEMD);
            SHA256_CTX ctxSHA2_224;
            SHA224_Init(&ctxSHA2_224);
            SHA256_CTX ctxSHA2_256;
            SHA256_Init(&ctxSHA2_256);
            SHA512_CTX ctxSHA2_384;
            SHA384_Init(&ctxSHA2_384);
            SHA512_CTX ctxSHA2_512;
            SHA512_Init(&ctxSHA2_512);
            WHIRLPOOL_CTX ctxWHIRLPOOL;
            WHIRLPOOL_Init(&ctxWHIRLPOOL);

            QByteArray outp;
            progressbar->reset();
            progressbar->setMaximum(100);
            qApp->processEvents();
            quint64 c = 0;

            while (!(line = pf.parseAll()).isEmpty())
            {
                if (report2.at(j).sizeItem()->text() == "MD5")
                {
                    MD5_Update(&ctxMD5, line, (size_t)line.size());
                }
                if (report2.at(j).sizeItem()->text() == "SHA-1")
                {
                    SHA1_Update(&ctxSHA, line, (size_t)line.size());
                }
                if (report2.at(j).sizeItem()->text() == "RIPEMD-160")
                {
                    RIPEMD160_Update(&ctxRIPEMD, line, (size_t)line.size());
                }
                if (report2.at(j).sizeItem()->text() == "SHA224")
                {
                    SHA224_Update(&ctxSHA2_224, line, (size_t)line.size());
                }
                if (report2.at(j).sizeItem()->text() == "SHA256")
                {
                    SHA256_Update(&ctxSHA2_256, line, (size_t)line.size());
                }
                if (report2.at(j).sizeItem()->text() == "SHA384")
                {
                    SHA384_Update(&ctxSHA2_384, line, (size_t)line.size());
                }
                if (report2.at(j).sizeItem()->text() == "SHA512")
                {
                    SHA512_Update(&ctxSHA2_512, line, (size_t)line.size());
                }
                if (report2.at(j).sizeItem()->text() == "WHIRLPOOL")
                {
                    WHIRLPOOL_Update(&ctxWHIRLPOOL, line, (size_t)line.size());
                }


                c += line.size();
                progressbar->setValue((c * 100 / pf.getFileSize()));
                qApp->processEvents();
                if (mStop)
                {
                    break;
                }
            }

            if (mStop)
            {
                ui->label_total_result->setText(tr("Total Result : Aborted"));
                pf.cosefile();
                return;
            }

            if (report2.at(j).sizeItem()->text() == "MD5")
            {
                MD5_Final(hashbuf, &ctxMD5);
                for (int i = 0; i < 16; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }
            if (report2.at(j).sizeItem()->text() == "SHA-1")
            {
                SHA1_Final(hashbuf, &ctxSHA);
                for (int i = 0; i < 20; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }
            if (report2.at(j).sizeItem()->text() == "RIPEMD-160")
            {
                RIPEMD160_Final(hashbuf, &ctxRIPEMD);
                for (int i = 0; i < 20; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }
            if (report2.at(j).sizeItem()->text() == "SHA224")
            {
                SHA224_Final(hashbuf, &ctxSHA2_224);
                for (int i = 0; i < 28; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }
            if (report2.at(j).sizeItem()->text() == "SHA256")
            {
                SHA256_Final(hashbuf, &ctxSHA2_256);
                for (int i = 0; i < 32; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }
            if (report2.at(j).sizeItem()->text() == "SHA384")
            {
                SHA384_Final(hashbuf, &ctxSHA2_384);
                for (int i = 0; i < 48; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }
            if (report2.at(j).sizeItem()->text() == "SHA512")
            {
                SHA512_Final(hashbuf, &ctxSHA2_512);
                for (int i = 0; i < 64; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }
            if (report2.at(j).sizeItem()->text() == "WHIRLPOOL")
            {
                WHIRLPOOL_Final(hashbuf, &ctxWHIRLPOOL);
                for (int i = 0; i < 64; i++ )
                {
                    outp[i] = hashbuf[i];
                }
            }

            if (mhashes.at(j) == outp.toHex())
            {
                 matches += 1;
                 ui->label_match->setText(tr("Parity : ") + QString::number(matches));
                 report2.at(j).statusItem()->setText(tr("MATCH"));
                 report2.at(j).statusItem()->setBackgroundColor(QColor(0, 255, 0, 30));
            } else {
                 missmatches += 1;
                 ui->label_missmatch->setText(tr("Failure : ") + QString::number(missmatches));
                 report2.at(j).statusItem()->setText(tr("FAILURE"));
                 report2.at(j).statusItem()->setBackgroundColor(QColor(255, 0, 0, 30));
            }
            pf.cosefile();
        } else {
            unreadable += 1;
            ui->label_unreadable->setText(tr("Unreadable : ") + QString::number(unreadable));
            report2.at(j).statusItem()->setText(tr("UNREADABLE"));
            report2.at(j).statusItem()->setBackgroundColor(QColor(255, 255, 0, 30));
            pf.cosefile();
        }
    }

    ui->labelStatus2->setText(tr("Done"));
    ui->label_total_files->setText(tr("Total Files : ") + QString::number(report2.size()));
    ui->label_match->setText(tr("Parity : ") + QString::number(matches));
    ui->label_missmatch->setText(tr("Failure : ") + QString::number(missmatches));
    ui->label_unreadable->setText(tr("Unreadable : ") + QString::number(unreadable));
    if (missmatches == 0 && unreadable == 0)
    {
        ui->label_total_result->setText("Total result : OK");
    } else {
        ui->label_total_result->setText("Total result : NOT OK");
    }
}

/*
 * Helper function to print report
 * in table widget 1 and 2.
 */
void MainWindow::printReport()
{
    if (ui->tabWidget->currentIndex() == 0)
    {
        ui->tableWidget->setSortingEnabled(false);
        ui->tableWidget->setRowCount(report.size());
        ui->tableWidget->setColumnCount(3);

        for (int i =  0; i < report.size(); i++)
        {
            if (mStop) { return; }
            qApp->processEvents();
            ui->tableWidget->setItem(i,0, report.at(i).fileItem());
            ui->tableWidget->setItem(i,1, report.at(i).sizeItem());
            ui->tableWidget->setItem(i,2, report.at(i).statusItem());
        }
        ui->tableWidget->resizeColumnToContents(0);
        ui->label_2->setText(tr("Files selected: ") + QString::number(report.size()));
        ui->tableWidget->setSortingEnabled(true);
    }

    if (ui->tabWidget->currentIndex() == 1)
    {
        ui->tableWidget_2->setSortingEnabled(false);
        ui->tableWidget_2->setRowCount(report2.size());

          for (int j =  0; j < report2.size(); j++)
          {
              ui->tableWidget_2->setItem(j,0, report2.at(j).fileItem());
              ui->tableWidget_2->setItem(j,1, report2.at(j).sizeItem());
              ui->tableWidget_2->setItem(j,2, report2.at(j).statusItem());
          }
          ui->tableWidget_2->resizeColumnToContents(0);
          ui->label_3->setText(tr("Files selected: ") + QString::number(report2.size()));
          ui->tableWidget_2->setSortingEnabled(true);
    }
}


void MainWindow::pre()
{
    mprogress = true;
    ui->menuBar->setDisabled(true);
    ui->groupBox_3->setDisabled(true);
    ui->commandLinkButtonRefresh->setDisabled(true);
    ui->commandLinkButtonClearAll->setDisabled(true);
    ui->commandLinkButtonRefresh_2->setDisabled(true);
    ui->commandLinkButtonClearAll_2->setDisabled(true);
    ui->commandLinkButtonSaveChecksum->setDisabled(true);
    ui->commandLinkButtonOpenCheckSumFile->setDisabled(true);
    ui->actionRefresh->setDisabled(true);
    ui->actionClear_All->setDisabled(true);
}

void MainWindow::post()
{
     mprogress = false;
     ui->menuBar->setDisabled(false);
     ui->groupBox_3->setDisabled(false);
     ui->commandLinkButtonRefresh->setDisabled(false);
     ui->commandLinkButtonClearAll->setDisabled(false);
     ui->commandLinkButtonRefresh_2->setDisabled(false);
     ui->commandLinkButtonClearAll_2->setDisabled(false);
     ui->commandLinkButtonSaveChecksum->setDisabled(false);
     ui->commandLinkButtonOpenCheckSumFile->setDisabled(false);
     ui->actionRefresh->setDisabled(false);
     ui->actionClear_All->setDisabled(false);
}

/*
 * Menubar Tools -> Properties was triggered
 */
void MainWindow::on_actionProperties_triggered()
{
    int ret;
    Properties propertiesDia;
    propertiesDia.setWindowTitle(tr("Properties"));
    propertiesDia.show();
    ret = propertiesDia.exec();
    switch(ret)
    {
        case 1:
            cf.readFile(QDir::home().absolutePath() + FILTER);
            m_hashtyp = cf.getDefaultHash();
            m_addRootPath = cf.isAddRootPath();
            m_rootPathTyp = cf.getRootPathTyp();
            m_style = cf.getStyle();
            ui->tabWidget->setCurrentIndex(0);
            init_hashtyp();
            break;
        case 0:
            break;
        default:
            break;
    }
}


/*
 * Helper function to avoid C2360
 */
void MainWindow::reInialise_ComboBox(void)
{
    /* ReInitialise filter combobox */
    ui->comboBox->clear();
    QListIterator<QString> i(m_key);

    while (i.hasNext())
    {
       QString filterkey = i.next();
       ui->comboBox->addItem(filterkey);
    }
}

/*
 * Helper function for init and properties
 */
void MainWindow::init_hashtyp()
{
    if (m_hashtyp == "MD5") ui->actionMD5->activate(QAction::Trigger);
    if (m_hashtyp == "SHA-1") ui->actionSHA_1->activate(QAction::Trigger);
    if (m_hashtyp == "RIPEMD-160") ui->actionRIPEMD_160->activate(QAction::Trigger);
    if (m_hashtyp == "SHA224") ui->actionSHA_2_224->activate(QAction::Trigger);
    if (m_hashtyp == "SHA256") ui->actionSHA_2_256->activate(QAction::Trigger);
    if (m_hashtyp == "SHA384") ui->actionSHA_2_384->activate(QAction::Trigger);
    if (m_hashtyp == "SHA512") ui->actionSHA_2_512->activate(QAction::Trigger);
    if (m_hashtyp == "WHIRLPOOL") ui->actionWhirlpool->activate(QAction::Trigger);
}

/*
 * Menubar Tools -> Filter was triggered
 */
void MainWindow::on_actionFilter_triggered()
{
    int ret;
    FilterSettings filterSettingsDialog;
    filterSettingsDialog.setWindowTitle(tr("Filter Settings"));
    filterSettingsDialog.show();
    ret = filterSettingsDialog.exec();
    switch(ret)
    {
        case 1:
            cf.readFile(QDir::home().absolutePath() + FILTER);
            m_includeFileFilter = cf.getIncludeFileFilter();
            m_excludeFileFilter = cf.getExcludeFileFilter();
            m_key = cf.getKey();
            reInialise_ComboBox();
            break;
        case 0:
            break;
        default:
            break;
    }
}

/*
 * Helper function to filter all
 * negative hits from file list
 */
void MainWindow::removeNegativeFiles(QList<ReportItem>& report)
{
    QRegExp pattern;
    QStringList negfilter = m_excludeFileFilter.values(ui->comboBox->currentText());
    QStringList tempList;
    bool gotyou = false;

    for (qint64 i = 0; i < report.size(); i++)
    {
        gotyou = false;
        for (qint64 j = 0; j < negfilter.size(); j++)
        {
            pattern.setPattern(negfilter.at(j));
            if (report.at(i).fileItem()->text().contains(pattern))
            {
                gotyou = true;
            }
        }

        if (!gotyou)
        {
            tempList.append(report.at(i).fileItem()->text());
        }
    }

    report.clear();
    for (int i = 0; i < tempList.size(); i++)
    {
        ReportItem r;
        r.fileItem()->setText(tempList.at(i));
        report.append(r);
    }
}

/*
 * Apply include and exclude filter
 * to print on table widget
 */
void MainWindow::sortreport()
{
    QList<ReportItem> temp;
    QStringList filter = m_includeFileFilter.values(ui->comboBox->currentText());
    QStringList negfilter = m_excludeFileFilter.values(ui->comboBox->currentText());
    QRegExp pattern;
    pattern.setPatternSyntax(QRegExp::WildcardUnix);
    pattern.setCaseSensitivity(Qt::CaseInsensitive);
    QRegExp antipattern;
    antipattern.setPatternSyntax(QRegExp::RegExp2);
    antipattern.setCaseSensitivity(Qt::CaseInsensitive);
    bool negativeHit = false;

    report.clear();

    for (int i = 0; i < root.size(); ++i)
    {
        if (mStop) { return; }
        qApp->processEvents();
        for(int j = 0; j < negfilter.size(); ++j)
        {
            antipattern.setPattern(negfilter.at(j));
            if (root.at(i).fileItem()->text().contains(antipattern))
            {
                negativeHit = true;
                break;
            }
        }

        if (!negativeHit)
        {
            for (int k = 0; k < filter.size(); ++k)
            {
                if (mStop) { return; }
                pattern.setPattern(filter.at(k));
                if(pattern.exactMatch(root.at(i).fileItem()->text()))
                {
                    ReportItem r;
                    r.filePath()->append(root.at(i).filePath());
                    r.fileItem()->setText(root.at(i).fileItem()->text());
                    r.sizeItem()->setText(root.at(i).sizeItem()->text());
                    r.statusItem()->setText(root.at(i).statusItem()->text());
                    temp.append(r);
                    break;
                }
            }
        } else {
            negativeHit = false;
        }
    }

    for (int i = 0; i < temp.size(); ++i)
    {
        if (mStop) { return; }
        qApp->processEvents();
        ReportItem r;
        r.filePath()->append(temp.at(i).filePath());
        r.fileItem()->setText(temp.at(i).fileItem()->text());
        r.sizeItem()->setText(temp.at(i).sizeItem()->text());
        r.statusItem()->setText(temp.at(i).statusItem()->text());
        report.append(r);
    }
}

void MainWindow::rootpath()
{
    if (ui->radioStatic->isChecked())
    {
        ui->labelRootPath->setText(mdir2.absolutePath() + "/");
        ui->labelRootPath2->setText(mdir2.absolutePath() + "/");
    }
    if (ui->radioByTag->isChecked())
    {
        QRegExp patternFILETag("^([^\\[]*)(\\[)([^\\]]*)(\\])(\\.*)(.*)$");
        patternFILETag.setPatternSyntax(QRegExp::RegExp2);
        QString rp;

        if (patternFILETag.indexIn(mchecksumfiles.at(0)) == 0)
        {
            QString filetag;
            filetag.append(patternFILETag.cap(3));
            rp = decodeDynamicPath(filetag);
            if (rp.contains("none"))
            {
                rp.clear();
                rp = decodeStaticPath(filetag);
            }
        } else {
            ui->radioDynamic->setChecked(true);
        }

        mdir2.setPath(rp);
        ui->labelRootPath->setText(mdir2.absolutePath() + "/");
        ui->labelRootPath2->setText(mdir2.absolutePath() + "/");
    }
    if (ui->radioDynamic->isChecked())
    {
        QFileInfo Info(QString(mchecksumfiles.at(0).data()));
        mdir2 = Info.dir();
        ui->labelRootPath->setText(mdir2.absolutePath() + "/");
        ui->labelRootPath2->setText(mdir2.absolutePath() + "/");
    }
    if (ui->radioNoRootPath->isChecked())
    {
            mdir2.setPath("");
            ui->labelRootPath->setText("");
            ui->labelRootPath2->setText("");
    }
}


/*
 * MD5 triggered
 */
void MainWindow::on_actionMD5_triggered()
{
    ui->tableWidget->horizontalHeaderItem(2)->setText(tr("MD5 Digest"));
    m_hashtyp = "MD5";
    ui->actionRefresh->activate(QAction::Trigger);
}

/*
 * SHA triggered
 */
void MainWindow::on_actionSHA_1_triggered()
{
    ui->tableWidget->horizontalHeaderItem(2)->setText(tr("SHA-1 Digest"));
    m_hashtyp = "SHA-1";
    ui->actionRefresh->activate(QAction::Trigger);
}

/*
 * RIPEMD-160 triggered
 */
void MainWindow::on_actionRIPEMD_160_triggered()
{
    ui->tableWidget->horizontalHeaderItem(2)->setText(tr("RIPEMD-160 Digest"));
    m_hashtyp = "RIPEMD-160";
    ui->actionRefresh->activate(QAction::Trigger);
}

/*
 * SHA224 triggered
 */
void MainWindow::on_actionSHA_2_224_triggered()
{
    ui->tableWidget->horizontalHeaderItem(2)->setText(tr("SHA224 Digest"));
    m_hashtyp = "SHA224";
    ui->actionRefresh->activate(QAction::Trigger);
}

/*
 * SHA256 triggered
 */
void MainWindow::on_actionSHA_2_256_triggered()
{
    ui->tableWidget->horizontalHeaderItem(2)->setText(tr("SHA256 Digest"));
    m_hashtyp = "SHA256";
    ui->actionRefresh->activate(QAction::Trigger);
}

/*
 * SHA384 triggered
 */
void MainWindow::on_actionSHA_2_384_triggered()
{
    ui->tableWidget->horizontalHeaderItem(2)->setText(tr("SHA384 Digest"));
    m_hashtyp = "SHA384";
    ui->actionRefresh->activate(QAction::Trigger);
}

/*
 * SHA512 triggered
 */
void MainWindow::on_actionSHA_2_512_triggered()
{
    ui->tableWidget->horizontalHeaderItem(2)->setText(tr("SHA512 Digest"));
    m_hashtyp = "SHA512";
    ui->actionRefresh->activate(QAction::Trigger);
}

/*
 * WHIRLPOOL triggered
 */
void MainWindow::on_actionWhirlpool_triggered()
{
    ui->tableWidget->horizontalHeaderItem(2)->setText(tr("WhirlPool Digest"));
    m_hashtyp = "WHIRLPOOL";
    ui->actionRefresh->activate(QAction::Trigger);
}


/*
 * Donation starts default web browser to donation url
 */
void MainWindow::on_actionDonate_triggered()
{
    QDesktopServices::openUrl(QUrl("http://www.checksumjack.org", QUrl::TolerantMode));
}

/**
 * This is called from main
 * if argument "-c" is passed to stdin
 * by the user.
 */
void MainWindow::callActionOpenChecksumFile(void)
{
    ui->actionOpen_Hash_File->activate(QAction::Trigger);
}

/**
 * This is called from main
 * if arguments "-md5" or "-sha1" are passed to stdin
 * by the user.
 */
void MainWindow::callActionOpenFile(QString &argument)
{
    QFileInfo info(m_arg.at(0));

    if (argument.contains("none"))
    {
            if (m_hashtyp == "MD5")
            {
                ui->actionMD5->activate(QAction::Trigger);
            }
            if (m_hashtyp == "SHA-1")
            {
                ui->actionSHA_1->activate(QAction::Trigger);
            }
            if (m_hashtyp == "RIPEMD-160")
            {
                ui->actionRIPEMD_160->activate(QAction::Trigger);
            }
            if (m_hashtyp == "SHA224")
            {
                ui->actionSHA_2_224->activate(QAction::Trigger);
            }
            if (m_hashtyp == "SHA256")
            {
                ui->actionSHA_2_256->activate(QAction::Trigger);
            }
            if (m_hashtyp == "SHA384")
            {
                ui->actionSHA_2_384->activate(QAction::Trigger);
            }
            if (m_hashtyp == "SHA512")
            {
                ui->actionSHA_2_512->activate(QAction::Trigger);
            }
            if (m_hashtyp == "WHIRLPOOL")
            {
                ui->actionWhirlpool->activate(QAction::Trigger);
            }
    }

    /* Also detects symlink to dir */
    if(info.isDir())
    {
        ui->actionOpen_dir->activate(QAction::Trigger);
    } else {
        ui->actionOpen_file->activate(QAction::Trigger);
    }
}

/*
 * F5 refresh is triggered
 */
void MainWindow::on_actionRefresh_triggered()
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        pre();
        calculateCheckSum();
        post();
        mStop = false;
    }
    if(ui->tabWidget->currentIndex() == 1 && !mchecksumfiles.isEmpty())
    {
        rootpath();
        pre();
        calculateValid();
        post();
        mStop = false;
    }
}

/*
 * Clear all is triggered (clear screen)
 */
void MainWindow::on_actionClear_All_triggered()
{
    if (ui->tabWidget->currentIndex() == 0)
    {
        report.clear();
        ui->label_2->setText(tr("Files selected: 0"));
        ui->labelStatus->setText("");
        ui->labelDir->setText("");
        ui->actionSave_Hash_File->setDisabled(true);
        ui->commandLinkButtonSaveChecksum->setDisabled(true);
        ui->groupBox->setTitle("Report");
        ui->checkBox->setHidden(true);
    }
    if (ui->tabWidget->currentIndex() == 1)
    {
        mdir.entryList().clear();
        mchecksumfiles.clear();
        report2.clear();
        ui->labelRootPath->setText("RootPath");
        ui->labelRootPath2->setText("");
        ui->labelStatus2->setText("");
        ui->label_3->setText(tr("Files selected: 0"));
        ui->label_total_files->setText(tr("Total Files :"));
        ui->label_match->setText(tr("Parity :"));
        ui->label_missmatch->setText(tr("Failure :"));
        ui->label_unreadable->setText(tr("Unreadable :"));
        ui->label_total_result->setText(tr("Total Result :"));
        ui->groupBox_5->setTitle("Report");
    }
    progressbar->reset();
    printReport();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 0)
    {
        if (!report.isEmpty())
        {
            ui->actionSave_Hash_File->setDisabled(false);
        }
        ui->tableWidget_2->setAcceptDrops(false);
        ui->tableWidget->setAcceptDrops(true);
        ui->menuHash->setDisabled(false);
        ui->actionOpen_dir->setDisabled(false);
        ui->actionOpen_file->setDisabled(false);
    }
    if (index == 1)
    {
        ui->tableWidget_2->setAcceptDrops(true);
        ui->tableWidget->setAcceptDrops(false);
        ui->menuHash->setDisabled(true);
        ui->actionSave_Hash_File->setDisabled(true);
        ui->actionOpen_Hash_File->setDisabled(false);
        ui->actionOpen_dir->setDisabled(true);
        ui->actionOpen_file->setDisabled(true);
    }
}


/*
 * Stop triggered (calculation is stoped)
 */
void MainWindow::on_actionStop_triggered()
{
    if (mprogress)
    {
        mStop = true;
    }
}

/*
 * Close Applikation Ctrl-Q (so long sucker)
 */
void MainWindow::on_actionQuit_triggered()
{
    exit(EXIT_SUCCESS);
}

/*
 * Show context menu (And start 'verify checksum' dialog)
 * right click on hash digest
 */
void MainWindow::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->tableWidget->mapToGlobal(pos);

    if (!ui->tableWidget->selectedItems().isEmpty())
    {
        if(ui->tableWidget->selectedItems().at(0)->column() == 0)
        {
            openSelected->setText(tr("Open"));
            checksumMenu->addAction(openSelected);
            checksumMenu->removeAction(verifyCheckSum);
            checksumMenu->exec(globalPos);
        }
        if(ui->tableWidget->selectedItems().at(0)->column() == 2)
        {
            verifyCheckSum->setText(tr("Compare Digest"));
            checksumMenu->addAction(verifyCheckSum);
            checksumMenu->removeAction(openSelected);
            checksumMenu->exec(globalPos);
        }
    }
}

void MainWindow::showContextMenu2(const QPoint &pos)
{
    QPoint globalPos2 = ui->tableWidget_2->mapToGlobal(pos);

    open2Selected->setText(tr("Open"));
    checksumMenu2->addAction(open2Selected);
    checksumMenu2->exec(globalPos2);

    if (ui->tableWidget_2->selectedItems().at(0)->column() == 0)
    {
        open2Selected->setText(tr("Open"));
        checksumMenu2->addAction(open2Selected);
        checksumMenu2->exec(globalPos2);
    }
}


/*
 * Start verify checksum dialog by double click on hash digetst
 */
void MainWindow::tableWidget_DoubleClick(QTableWidgetItem *t)
{
    if (t->column() == 0)
    {
        emit openSelected_triggered();
    }
    if (t->column() == 2)
    {
        emit verifyCheckSum_triggered();
    }
}

/*
 * Start verify checksum dialog
 * double click or context menu start this
 */
void MainWindow::verifyCheckSum_triggered()
{
    int ret;
    QString x = ui->tableWidget->selectedItems().at(0)->text();
    VerifyDialog verifyDialog;
    verifyDialog.setCheckSum1(x);
    verifyDialog.setWindowTitle("Compare Digest");
    verifyDialog.show();
    ret = verifyDialog.exec();
}

/*
 * context menu open
 * open selected files by os default
 * applikation
 */
void MainWindow::openSelected_triggered()
{
    QList<QTableWidgetItem *> selected = ui->tableWidget->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {
        QString file("file:///");
        file.append(ui->labelDir->text());
        file.append(item->text());
        QDesktopServices::openUrl(QUrl(file));
    }
}

/*
 * context menu open
 * tableWidget 2
 * open selected files by os default
 * applikation
 */
void MainWindow::open2Selected_triggered()
{
    QList<QTableWidgetItem *> selected = ui->tableWidget_2->selectedItems();
    QTableWidgetItem *item;

    foreach (item, selected) {
        QString file("file:///");
        file.append(ui->labelRootPath->text());
        file.append(item->text());
        QDesktopServices::openUrl(QUrl(file));
    }
}

void MainWindow::dropEvent(QDropEvent *de)
{
    QStringList mylist;
    QList<QUrl> urls = de->mimeData()->urls();
    if (urls.isEmpty()) return;

    for (int i = 0; i < urls.size(); ++i)
    {
        mylist.append(urls.at(i).toLocalFile());
    }

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

    if (mylist.first().contains(patternMD5ending) ||
        mylist.first().contains(patternSHA1ending) ||
        mylist.first().contains(patternRIPEMD160ending) ||
        mylist.first().contains(patternSHA2_224ending) ||
        mylist.first().contains(patternSHA2_256ending) ||
        mylist.first().contains(patternSHA2_384ending) ||
        mylist.first().contains(patternSHA2_512ending) ||
        mylist.first().contains(patternWHIRLPOOLending))
    {
        ui->tabWidget->setCurrentIndex(1);
    } else {
        ui->tabWidget->setCurrentIndex(0);
    }

    this->drag_checksumfiles(mylist);
}

void MainWindow::dragMoveEvent(QDragMoveEvent *de)
{
    /* The event needs to be accepted here */
    de->accept();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    /* Set the drop action to be the proposed action. */
    event->acceptProposedAction();
}


void MainWindow::drag_checksumfiles(QStringList &files)
{
    QFileInfo info(files.first());

    if (ui->tabWidget->currentIndex() == 0)
    {
        if (info.isDir())
        {
            mdragflag = true;
            mdir.entryList().clear();
            if (info.isSymLink())
            {
                mdir.setPath(info.symLinkTarget());
            } else {
                mdir.setPath(info.absoluteFilePath());
            }
            ui->actionOpen_dir->activate(QAction::Trigger);
        }
        if (info.isFile()) {
            ui->checkBox->setHidden(true);
            m_arg = files;
            ui->actionOpen_file->activate(QAction::Trigger);
        }
    }

    if (ui->tabWidget->currentIndex() == 1)
    {
        m_arg = files;
        ui->actionOpen_Hash_File->activate(QAction::Trigger);
    }
}

/*
 * Radiobutton 1
 * Dont use any root path at all
 */
void MainWindow::on_radioNoRootPath_clicked()
{
    ui->toolButton->setEnabled(false);
    ui->actionRefresh->activate(QAction::Trigger);
}

/*
 * Radiobutton 2
 * Use same rootpath as hash file's current path
 */
void MainWindow::on_radioDynamic_clicked()
{
    ui->toolButton->setEnabled(false);
    ui->actionRefresh->activate(QAction::Trigger);
}

/*
 * Radiobutton 3
 * Use file name tag [xyz] as root path
 */
void MainWindow::on_radioByTag_clicked()
{
    ui->toolButton->setEnabled(false);
    ui->actionRefresh->activate(QAction::Trigger);
}

/*
 * Radiobutton 4
 * Use user definded root path
 */
void MainWindow::on_radioStatic_clicked(bool checked)
{
    if(checked)
    {
        ui->toolButton->setEnabled(true);
    } else {
        ui->toolButton->setEnabled(false);
    }
}

/*
 * User starts QFileDialog to choose root path (simple PushButton)
 */
void MainWindow::on_toolButton_clicked()
{
        mdir2 = QFileDialog::getExistingDirectory(this, tr("Select Root Path directory"),
                                                  QDir::homePath(),
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        ui->labelRootPath->setText(mdir2.absolutePath() + "/");
        ui->labelRootPath2->setText(mdir2.absolutePath()+ "/");
        ui->actionRefresh->activate(QAction::Trigger);
}

