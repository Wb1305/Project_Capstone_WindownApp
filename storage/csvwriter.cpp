#include "csvwriter.h"
#include <QDir>
#include <QDebug>

// CsvWriter::CsvWriter() {}

CsvWriter::CsvWriter(const QString &filePath) :
    m_filePath(filePath)
{
    QFileInfo info(m_filePath);
    QDir().mkpath(info.absolutePath());
}

CsvWriter::~CsvWriter()
{

}

void CsvWriter::appendRow(const QStringList &row)
{
    QMutexLocker locker(&m_mutex);

    QFile file(m_filePath);

    if(!file.open(QIODevice::Append | QIODevice::Text)){

        qWarning()<< "[CsvWriter] Can't opend file to write:" << m_filePath;
        return;
    }
    QTextStream stream(&file);
    stream << row.join(',') << "\n";
    file.flush();
}

void CsvWriter::appendHeader(const QStringList &header)
{
    QMutexLocker locker(&m_mutex);

    QFile file(m_filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << header.join(',') << "\n";
    }
}

bool CsvWriter::isFileExists() const
{
    return QFile::exists(m_filePath);
}
