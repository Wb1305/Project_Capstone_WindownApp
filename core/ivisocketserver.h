#ifndef IVISOCKETSERVER_H
#define IVISOCKETSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

#include <QObject>

class IviSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit IviSocketServer(QObject *parent = nullptr);
    ~IviSocketServer();

    void startListening(quint16 port);
    void startListeningAt(const QHostAddress& address, quint16 port);
    void stopListening();
    bool isListening() const;
    void sendData(const QByteArray& data);
    //test data nhận được
    void printRawData(const QByteArray& data);

signals:
    void dataReceived(const QByteArray& data);
    void clientConnected();
    void clientDisconnected();
    void sendFailed(const QString& reason);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpServer* m_server = nullptr;
    QTcpSocket* m_clientSocket = nullptr;
    quint16 m_port = 0;
    QByteArray m_buffer;
    QHostAddress m_hostAddress;
};

#endif // IVISOCKETSERVER_H
