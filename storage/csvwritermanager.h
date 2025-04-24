#ifndef CSVWRITERMANAGER_H
#define CSVWRITERMANAGER_H

#include <QString>
#include <QMap>
#include "storage/csvwriter.h"

class CsvWriterManager
{
public:
    explicit CsvWriterManager(const QString& baseDir);
    ~CsvWriterManager();

    CsvWriter* createCsvWriter(const QString& baseName, const QDate& date);

private:
    QString m_baseDir;
    QMap<QString, CsvWriter*> m_writers;
};

#endif // CSVWRITERMANAGER_H
