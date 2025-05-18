#include "csvwritermanager.h"
#include <qdatetime.h>
#include <QDebug>

// CsvWriterManager::CsvWriterManager() {}

CsvWriterManager::CsvWriterManager(const QString &baseDir) :
    m_baseDir(baseDir)
{

}

CsvWriterManager::~CsvWriterManager()
{
    for (CsvWriter* writer : std::as_const(m_writers))
        delete writer;
}

CsvWriter *CsvWriterManager::createCsvWriter(const QString &baseName, const QDate &date)
{
    QString filename = QString("%1_%2.csv").arg(baseName, date.toString("yyyy-MM-dd"));

    if (!m_writers.contains(filename)) {
        QString fullPath = m_baseDir + "/" + filename;
        m_writers[filename] = new CsvWriter(fullPath);

        // qDebug()<<"[CsvWriterManager] fullpath: "<< fullPath;
    }

    return m_writers[filename];
}
