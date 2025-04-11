#ifndef PROCESSLISTMODEL_H
#define PROCESSLISTMODEL_H

#include <QObject>
#include <QAbstractTableModel>

class ProcessListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ProcessListModel(QObject *parent = nullptr);

signals:
};

#endif // PROCESSLISTMODEL_H
