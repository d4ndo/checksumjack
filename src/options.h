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
    QString style() const;
    void setStyle(const QString &style);
    void apply();

signals:
    void initProperties();
    void init_hashtyp();

public slots:

private:
    bool m_addRootPath;
    QString m_defaultHash;
    QString m_rootPathTyp;
    QString m_style;

};

#endif // OPTIONS_H
