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

signals:

public slots:

private:
    bool m_addRootPath;


};

#endif // OPTIONS_H
