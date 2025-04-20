#include "ivisocketserver.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>


IviSocketServer::IviSocketServer(QObject *parent)
    : QObject{parent}, m_server(new QTcpServer(this))
{
    connect(m_server, &QTcpServer::newConnection, this, &IviSocketServer::onNewConnection);
    // connect(this, &IviSocketServer::dataReceived, this, &IviSocketServer::printRawData);
}

IviSocketServer::~IviSocketServer()
{
    stopListening();
}

void IviSocketServer::startListening(quint16 port)
{
    m_port = port;
    if(!m_server->listen(QHostAddress::Any, port)){
        qWarning()<<"Failed to listen on port:" << port << m_server->errorString();
        return;
    }
    qDebug() << "TCP Server listening on port:" << port;
}

// void IviSocketServer::startListeningAt(const QHostAddress &address, quint16 port)
// {
//     if (m_server->isListening()) {
//         m_server->close(); // Dừng lắng nghe nếu đang mở cổng cũ
//     }

//     if (!m_server->listen(address, port)) {
//         emit sendFailed(QString("Cannot listen at %1:%2 - %3")
//                             .arg(address.toString())
//                             .arg(port)
//                             .arg(m_server->errorString()));
//         return;
//     }

//     m_port = port;
//     connect(m_server, &QTcpServer::newConnection, this, &IviSocketServer::onNewConnection);

//     qDebug() << "[TCPServer] Listening now: " << address.toString() << ":" << port;
// }

void IviSocketServer::stopListening()
{
    if(m_clientSocket){
        m_clientSocket->disconnectFromHost();
        m_clientSocket->deleteLater();
        m_clientSocket = nullptr;
    }
    m_server->close();
    qDebug() << "TCP Server stopped";
}

bool IviSocketServer::isListening() const
{
    return m_server->isListening();
}

void IviSocketServer::sendCommandToLinux(const QByteArray &data)
{
    if(!m_clientSocket){
        emit sendFailed("No client connected.");
        return;
    }
    if(m_clientSocket->state() != QAbstractSocket::ConnectedState){
        emit sendFailed("Client is not connected.");
        return;
    }
    qDebug()<<"Sending comman to linux: "<< data;

    m_clientSocket->write(data + "\n");
}

bool IviSocketServer::isValidJson(const QByteArray &buffer)
{
    QJsonParseError error;
    QJsonDocument::fromJson(buffer, &error);

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "[IviSocketServer] Error parse JSON:" << error.errorString();
        return false;
    }
    return true;
}

void IviSocketServer::printRawData(const QByteArray &data)
{
    qDebug().noquote()<<"[Raw Data Received]\n"<<QString::fromUtf8(data);
}

void IviSocketServer::onCommandReceived(const QByteArray &commandJson)
{
    sendCommandToLinux(commandJson);
}

void IviSocketServer::onNewConnection()
{
    if(m_clientSocket){
        m_clientSocket->disconnectFromHost();
        m_clientSocket->deleteLater();
    }
    m_clientSocket = m_server->nextPendingConnection();
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &IviSocketServer::onReadyRead);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &IviSocketServer::onDisconnected);
    qDebug() << "Client connected from" << m_clientSocket->peerAddress().toString();
    emit clientConnected();

    // Gửi lệnh "kill" sau 10 giây
    // QTimer::singleShot(4000, this, [this](){
    //     qDebug()<<"=== Kill proesses ===";
    //     QJsonObject obj;
    //     obj["type"] = "killProcess";

    //     QJsonArray pnameList;
    //     pnameList << "brave" <<"vlc";
    //     obj["PNames"] = pnameList;

    //     QJsonDocument doc(obj);
    //     QByteArray command = doc.toJson(QJsonDocument::Compact);

    //     sendCommandToLinux(command);
    // });

}

void IviSocketServer::onReadyRead()
{
    if (!m_clientSocket) {
        qDebug() << "[IviSocketServer] Error: Socket does not exist!";
        return;
    }

    while(m_clientSocket->canReadLine()){
        QByteArray buffer = m_clientSocket->readLine().trimmed();

        if (isValidJson(buffer)) {
            emit dataReceived(buffer);
        } else {
            qDebug() << "[IviSocketServer] JSON does not valid!";
        }
    }

    // m_buffer.append(m_clientSocket->readAll());

    // if (isValidJson(m_buffer)) {
    //     emit dataReceived(m_buffer);
    //     m_buffer.clear();
    // } else {
    //     qDebug() << "[IviSocketServer] JSON does not valid!";
    // }
}

void IviSocketServer::onDisconnected()
{
    qDebug() << "Client disconnected.";
    emit clientDisconnected();

    m_clientSocket->deleteLater();
    m_clientSocket = nullptr;
}
