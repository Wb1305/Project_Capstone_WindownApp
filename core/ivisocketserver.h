#ifndef IVISOCKETSERVER_H
#define IVISOCKETSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
#include <QObject>

class IviSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit IviSocketServer(QObject *parent = nullptr);
    ~IviSocketServer();

    void startListening(QString hostAddress, quint16 port);
    void stopListening();
    void sendCommandToLinux(const QByteArray& data);

    //test data nhận được
    void printRawData(const QByteArray& data);

private:
    bool isListening() const;
    bool isValidJson(const QByteArray& data);

signals:
    void dataReceived(const QByteArray& data);
    void clientConnected();
    void clientDisconnected();
    void sendFailed(const QString& reason);

public slots:
    void onCommandReceived(const QByteArray &commandJson);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
    void onDiscoveryRequest();

private:
    QTcpServer* m_server = nullptr;
    QTcpSocket* m_clientSocket = nullptr;
    quint16 m_port = 0;
    QString m_hostAddress;

    QUdpSocket* m_discoverySocket = nullptr;
};

#endif // IVISOCKETSERVER_H
