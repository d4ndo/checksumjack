#ifndef OPTIONS_H
#define OPTIONS_H

#include <QSettings>

class Options : public QObject
{
    Q_OBJECT
public:
    explicit Options(QObject *parent = 0);

    void readSettings();
    void writeSettings();

    bool addRootPath() const;
    void setAddRootPath(bool addRootPath);
    QString defaultHash() const;
    void setDefaultHash(const QString &defaultHash);
    QString rootPathTyp() const;
    void setRootPathTyp(const QString &rootPathTyp);
    QString format() const;
    void setFormat(const QString &format);
    bool fullPath() const;
    void setFullPath(bool fullPath);
    void apply();

signals:
    void initProperties();
    void init_hashtyp();

public slots:

private:
    bool m_addRootPath;
    QString m_defaultHash;
    QString m_rootPathTyp;
    QString m_format;
    bool m_fullPath;

};

#endif // OPTIONS_H
