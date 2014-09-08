#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "../src/fileio.h"
#include "../src/hashfileio.h"
#include <QDebug>
#include <QDir>


class unitTest : public QObject
{
    Q_OBJECT

public:
    unitTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testFileIO();
    void testFileIO_data();
};

unitTest::unitTest()
{
}

void unitTest::initTestCase()
{
    QFile file(QDir::tempPath() + "/" + "fileio.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << "The answer to the ultimate question of life the universe and everything: " << 42 << "\n";
    file.close();
}

void unitTest::cleanupTestCase()
{    
}

void unitTest::testFileIO()
{
    FileIO f;
    QByteArray z = "0";
    f.setFileName(QDir::tempPath() + "/" + "fileio.txt");
    QVERIFY2(f.openFileRead() == true, "Failure to open file for reading");
    QCOMPARE(f.getFileName(), QString(QDir::tempPath() + "/" + "fileio.txt"));
    QVERIFY2(f.getFileSize() == 76, "File size is not correct");
    QVERIFY2(f.parseAll() != z, "parseALL failed");
    f.cosefile();
}

void unitTest::testFileIO_data()
{
    //QTest::addColumn<FileIO>("stream");
    //QTest::newRow("fileio OK") << "fileio.txt";
}

QTEST_MAIN(unitTest)

#include "tst_unittest.moc"
