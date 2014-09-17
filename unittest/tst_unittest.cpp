#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "../src/fileio.h"
#include "../src/hashfileio.h"
#include "../src/globaldefs.h"
#include "../src/detector.h"
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
    void testHashFileWrite();
    void testDetectorHashDigest();
    void testDetectorHashDigest_data();
    void testDetectorHashTyp();
    void testDetectorHashTyp_data();
    void testDetectorFileName();
    void testDetectorFileName_data();
};

unitTest::unitTest()
{
}

void unitTest::initTestCase()
{
    /* Some test file for testFileIO */
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
    QVERIFY2(f.openFileRead() == true ,"Failure to open file for reading");
    QCOMPARE(f.getFileName(), QString(QDir::tempPath() + "/" + "fileio.txt"));
    QVERIFY2(f.getFileSize() == 76, "File size is not correct");
    QVERIFY2(f.parseAll() != z, "parseALL failed");
    f.cosefile();
}

void unitTest::testHashFileWrite()
{
    HashFileIO hf("foo");
    /* Testing the getter and setter */
    QCOMPARE(hf.getformat(), QString("gnu"));
    QCOMPARE(hf.getfullPath(), false);
    hf.setformat("bsd");
    QCOMPARE(hf.getformat(), QString("bsd"));
    hf.setfullPath(true);
    QCOMPARE(hf.getfullPath(), true);
    hf.setHashFileName("bar");
    QCOMPARE(hf.getHashFileName(), QString("bar"));

    /* Test writing gnu */
    struct hashSet hashset;
    hf.setHashFileName(QDir::tempPath() + "/" + "hashfileio.sha256");
    hf.openHashFileWriteing();
    /* open with Qt api */
    QFile file(QDir::tempPath() + "/" + "hashfileio.sha256");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);

    hf.setformat("gnu");
    hf.setfullPath(false);
    hashset.hash = QString("123");
    hashset.hashtyp = "SHA256";
    hashset.rootpath = "/tmp/";
    hashset.file = "somefile";
    hf.writerToHashFile(hashset);
    QString line = in.readLine();
    QCOMPARE(line, QString("123 somefile"));

    hf.setformat("gnu");
    hf.setfullPath(true);
    hashset.hash = "123";
    hashset.hashtyp = "SHA256";
    hashset.rootpath = "/tmp/";
    hashset.file = "somefile";
    hf.writerToHashFile(hashset);
    line = in.readLine();
    QCOMPARE(line, QString("123 /tmp/somefile"));

    hf.setformat("bsd");
    hf.setfullPath(false);
    hashset.hash = "123";
    hashset.hashtyp = "SHA256";
    hashset.rootpath = "/tmp/";
    hashset.file = "somefile";
    hf.writerToHashFile(hashset);
    line = in.readLine();
    QCOMPARE(line, QString("SHA256 (somefile) = 123"));

    hf.setformat("bsd");
    hf.setfullPath(true);
    hashset.hash = "123";
    hashset.hashtyp = "SHA256";
    hashset.rootpath = "/tmp/";
    hashset.file = "somefile";
    hf.writerToHashFile(hashset);
    line = in.readLine();
    QCOMPARE(line, QString("SHA256 (/tmp/somefile) = 123"));

    hf.setformat("csv");
    hf.setfullPath(false);
    hashset.hash = "123";
    hashset.hashtyp = "SHA256";
    hashset.rootpath = "/tmp/";
    hashset.file = "somefile";
    hf.writerToHashFile(hashset);
    line = in.readLine();
    QCOMPARE(line, QString("123,somefile"));

    hf.setformat("csv");
    hf.setfullPath(true);
    hashset.hash = "123";
    hashset.hashtyp = "SHA256";
    hashset.rootpath = "/tmp/";
    hashset.file = "somefile";
    hf.writerToHashFile(hashset);
    line = in.readLine();
    QCOMPARE(line, QString("123,/tmp/somefile"));

    hf.closeHashFile();
    file.close();
}

void unitTest::testDetectorHashDigest_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("bsd hash digest normal") << "MD5 (datei.txt) = 9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a" << "9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a";
    QTest::newRow("bsd hash digest tab1") << "md5    (datei.txt) = 9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a" << "9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a";
    QTest::newRow("bsd hash digest tab2") << "SHA1 (datei.txt)  = 9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a" << "9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a";
    QTest::newRow("bsd hash digest tab3") << "RIPEMD160 (datei.txt) =   9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a" << "9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a";
    QTest::newRow("bsd hash digest whitespace 0") << "SHA1 ( datei.txt) = 9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a" << "9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a";
    QTest::newRow("bsd hash digest whitespace 1") << "RIPEMD-160       (datei.txt) = 9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a" << "9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a";
    QTest::newRow("bsd hash digest whitespace 2") << "SHA224 (datei.txt)     = 9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a" << "9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a";
    QTest::newRow("bsd hash digest whitespace 3") << "SHA-224 (datei.txt) =        9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a" << "9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a";
    QTest::newRow("bsd hash no valid hash") << "SHA256 (datei.txt) = 69a" << "none";
    QTest::newRow("gnu hash digest") << "9e3d74e349767bbd1a87c19e082efe3a378e1bc2273b333ee8b1cc00254557fa filename" << "";
}

void unitTest::testDetectorHashDigest()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    QCOMPARE(detectHASHDigest(input), output);
}

void unitTest::testDetectorHashTyp_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("bsd hash digest MD5") << "MD5 (file.txt) = testpruefsummehashhashhas9hashhash" << "MD5";
    QTest::newRow("bsd hash digest md5") << "md5 (file.txt) = testpruefsummehashhashhas9hashhash" << "MD5";
    QTest::newRow("bsd hash digest SHA1") << "SHA1 (file.txt) = testpruefsummehashhashhas9hashhash" << "SHA-1";
    QTest::newRow("bsd hash digest SHA-1") << "SHA-1 (file.txt) = testpruefsummehashhashhas9hashhash" << "SHA-1";
    QTest::newRow("bsd hash digest RIPEMD160") << "RIPEMD160 (file.txt) = testpruefsummehashhashhas9hashhash" << "RIPEMD-160";
    QTest::newRow("bsd hash digest RMD-160") << "RMD160 (file.txt) = testpruefsummehashhashhas9hashhash" << "RIPEMD-160";
    QTest::newRow("bsd hash digest SHA224") << "SHA224 (file.txt) = testpruefsummehashhashhas9hashhash" << "SHA224";
    QTest::newRow("bsd hash digest SHA-224") << "SHA-224 (file.txt) = testpruefsummehashhashhas9hashhash" << "SHA224";
    QTest::newRow("bsd hash digest SHA256") << "SHA256 (file.txt) = testpruefsummehashhashhas9hashhash" << "SHA256";
    QTest::newRow("bsd hash digest SHA-256") << "SHA-256 (file.txt) = testpruefsummehashhashhas9hashhash" << "SHA256";
    QTest::newRow("bsd hash digest SHA384") << "SHA384 (file.txt) = testpruefsummehashhashhas9hashhash" << "SHA384";
    QTest::newRow("bsd hash digest SHA-384") << "SHA-384 (file.txt) = testpruefsummehashhashhas9hashhash" << "SHA384";
    QTest::newRow("bsd hash digest SHA512") << "SHA512 (file.txt) = testpruefsummehashhashhas9hashhash" << "SHA512";
    QTest::newRow("bsd hash digest SHA-512") << "SHA-512 (file.txt) = testpruefsummehashhashhas9hashhash" << "SHA512";
    QTest::newRow("bsd hash digest WHIRLPOOL") << "WHIRLPOOL (file.txt) = testpruefsummehashhashhas9hashhash" << "WHIRLPOOL";
    QTest::newRow("bsd hash digest whirlpool") << "whirlpool (file.txt) = testpruefsummehashhashhas9hashhash" << "WHIRLPOOL";
    QTest::newRow("bsd hash no valid hash") << "SHB256 (file.txt) = 69a" << "none";
}

void unitTest::testDetectorHashTyp()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    QCOMPARE(detectHashTyp(input), output);
}


void unitTest::testDetectorFileName_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("bsd hash filename 0") << "MD5 (file.txt) = testpruefsummehashhashhas9hashhash" << "file.txt";
    QTest::newRow("bsd hash filename 1") << "MD5 (./file.txt) = testpruefsummehashhashhas9hashhash" << "./file.txt";
    QTest::newRow("bsd hash filename 2") << "MD5 (./DIR/file.txt) = testpruefsummehashhashhas9hashhash" << "./DIR/file.txt";
    QTest::newRow("bsd hash filename 3") << "MD5 (file.txt) = testpruefsummehashhashhas9hashhash" << "file.txt";
    QTest::newRow("bsd hash filename 4") << "MD5 (C:\file.txt) = testpruefsummehashhashhas9hashhash" << "C:\file.txt";
    QTest::newRow("bsd hash filename 5") << "MD5 (C:\file/ /space.txt) = testpruefsummehashhashhas9hashhash" << "C:\file/ /space.txt";
    QTest::newRow("gnu hash filename 0") << "testpruefsummehashhashhas9hashhash file.txt" << "file.txt";
    QTest::newRow("gnu hash filename 1") << "testpruefsummehashhashhas9hashhash     file.txt" << "file.txt";
    QTest::newRow("gnu hash filename 2") << "testpruefsummehashhashhas9hashhash ./file.txt" << "./file.txt";
    QTest::newRow("gnu hash filename 3") << "testpruefsummehashhashhas9hashhash ./DIR/file.txt" << "./DIR/file.txt";
    QTest::newRow("gnu hash filename 4") << "testpruefsummehashhashhas9hashhash C:\file.txt" << QString("C:\file.txt");
    QTest::newRow("gnu hash filename 5") << "testpruefsummehashhashhas9hashhash C:\file/ /space.txt" << QString("C:\file/ /space.txt");
}

void unitTest::testDetectorFileName()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    QCOMPARE(detectFilename(input), output);
}


QTEST_MAIN(unitTest)

#include "tst_unittest.moc"
