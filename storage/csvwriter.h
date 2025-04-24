#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QMutexLocker>

class CsvWriter
{
public:
    explicit CsvWriter(const QString &filePath);
    ~CsvWriter();

    void appendRow(const QStringList &row);
    void appendHeader(const QStringList& header);
    bool isFileExists() const;

private:
    QString m_filePath;
    QMutex m_mutex;
};

#endif // CSVWRITER_H
