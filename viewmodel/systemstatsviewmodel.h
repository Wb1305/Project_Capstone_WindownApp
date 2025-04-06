#ifndef SYSTEMSTATSVIEWMODEL_H
#define SYSTEMSTATSVIEWMODEL_H

#include <QObject>

class SystemStatsViewModel : public QObject
{
    Q_OBJECT
public:
    explicit SystemStatsViewModel(QObject *parent = nullptr);

signals:
};

#endif // SYSTEMSTATSVIEWMODEL_H
