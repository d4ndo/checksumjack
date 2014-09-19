#ifndef DETECTOR_H
#define DETECTOR_H

#include "globaldefs.h"
#include <QString>
#include <QRegExp>

QString detectHASHDigest(const QString& input);
QString detectHashTyp(const QString& input);
QString detectFilename(const QString& input);
bool detectCommentOrEmpty(const QString& input);
//QString dectectHashTypByFile(const QString& filename);


#endif // DETECTOR_H
