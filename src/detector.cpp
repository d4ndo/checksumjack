
#include <QString>
#include <QRegExp>
#include <QDir>

/* Detect hash digest from hashfile hashset */
QString detectHASHDigest(const QString &input)
{
    QRegExp patternMD5Format("^([^\\s]{32,128})(\\s+)(\\*{0,1})(.*)$");
    QRegExp patternBSDFormat("^([^\\s]*)(\\s*\\()([^\\)]*)(\\)\\s*)(=\\s*)(.{32,128})$");
    patternMD5Format.setPatternSyntax(QRegExp::RegExp2);
    patternBSDFormat.setPatternSyntax(QRegExp::RegExp2);

    if (patternBSDFormat.indexIn(input) == 0)
    {
        /* BSD Format detected */
        /* capture hash */
        return patternBSDFormat.cap(6);
    } else if (patternMD5Format.indexIn(input) == 0) {
        /* GNU Format detected */
        return patternBSDFormat.cap(6);
    }
    return "none";
}

/* Detect hash typ from hashfile hashset */
QString detectHashTyp(const QString &input)
{
    QRegExp patternBSDFormat("^([^\\s]*)(\\s*\\()([^\\)]*)(\\)\\s*)(=\\s*)(.{32,128})$");
    patternBSDFormat.setPatternSyntax(QRegExp::RegExp2);
    patternBSDFormat.setCaseSensitivity(Qt::CaseInsensitive);

    if (patternBSDFormat.indexIn(input) == 0)
    {
        /* BSD Format detected */
        /* capture hashtyp */
        if (patternBSDFormat.cap(1) == "MD5" || patternBSDFormat.cap(1) == "md5")  return "MD5";
        if (patternBSDFormat.cap(1) == "SHA1" || patternBSDFormat.cap(1) == "SHA-1" ||
            patternBSDFormat.cap(1) == "sha1" || patternBSDFormat.cap(1) == "sha-1") return "SHA-1";
        if (patternBSDFormat.cap(1) == "RIPEMD160" || patternBSDFormat.cap(1) == "ripemd160" ||
            patternBSDFormat.cap(1) == "RIPEMD-160" || patternBSDFormat.cap(1) == "RMD160" ||
            patternBSDFormat.cap(1) == "ripemd-160" || patternBSDFormat.cap(1) == "rmd160") return "RIPEMD-160";
        if (patternBSDFormat.cap(1) == "SHA224" || patternBSDFormat.cap(1) == "SHA-224" ||
            patternBSDFormat.cap(1) == "sha224" || patternBSDFormat.cap(1) == "sha-224") return "SHA224";
        if (patternBSDFormat.cap(1) == "SHA256" || patternBSDFormat.cap(1) == "SHA-256" ||
            patternBSDFormat.cap(1) == "sha256" || patternBSDFormat.cap(1) == "sha-256") return "SHA256";
        if (patternBSDFormat.cap(1) == "SHA384" || patternBSDFormat.cap(1) == "SHA-384" ||
            patternBSDFormat.cap(1) == "sha384" || patternBSDFormat.cap(1) == "sha-384") return "SHA384";
        if (patternBSDFormat.cap(1) == "SHA512" || patternBSDFormat.cap(1) == "SHA-512" ||
            patternBSDFormat.cap(1) == "sha512" || patternBSDFormat.cap(1) == "sha-512") return "SHA512";
        if (patternBSDFormat.cap(1) == "WHIRLPOOL" || patternBSDFormat.cap(1) == "whirlpool") return "WHIRLPOOL";
    }
    return "none";
}

/* Detect filename form hashfile hash set */
QString detectFilename(const QString &input)
{
    QRegExp patternMD5Format("^([^\\s]{32,128})(\\s+)(\\*{0,1})(.*)$");
    QRegExp patternBSDFormat("^([^\\s]*)(\\s*\\()([^\\)]*)(\\)\\s*)(=\\s*)(.{32,128})$");
    patternMD5Format.setPatternSyntax(QRegExp::RegExp2);
    patternBSDFormat.setPatternSyntax(QRegExp::RegExp2);

    if (patternBSDFormat.indexIn(input) == 0)
    {
        /* BSD Format detected */
        /* capture filename */
        return QDir::fromNativeSeparators(patternBSDFormat.cap(3).trimmed());
    } else if (patternMD5Format.indexIn(input) == 0) {
        /* GNU Format detected */
        return QDir::fromNativeSeparators(patternMD5Format.cap(4).trimmed());
    }
    return "none";
}

bool detectCommentOrEmpty(const QString &input)
{
    QRegExp patternComment("^\\s*[;|//].*$");
    QRegExp patternEmpty("^\\s*$");

    if (patternComment.indexIn(input) != 0 && patternEmpty.indexIn(input) != 0) {
        return true;
    }
    return false;
}



