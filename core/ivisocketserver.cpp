#include "ivisocketserver.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QNetworkInterface>
#include <QNetworkDatagram>


IviSocketServer::IviSocketServer(QObject *parent)
    : QObject{parent},
    m_server(new QTcpServer(this))
{
    connect(m_server, &QTcpServer::newConnection, this, &IviSocketServer::onNewConnection);
}

IviSocketServer::~IviSocketServer()
{
    stopListening();
}

void IviSocketServer::startListening(QString hostAddress, quint16 port)
{
    if(port != m_port)
        m_port = port;
    if(hostAddress != m_hostAddress)
        m_hostAddress = hostAddress;

    if(!m_server->listen(QHostAddress::Any, port)){
        qWarning()<<"Failed to listen on port:" << port << m_server->errorString();
        return;
    }
    qDebug() << "TCP Server listening on Host: " << hostAddress << " and Port: "<<port;

    // Khởi động UDP socket để lắng nghe DISCOVER_SERVER
    if (!m_discoverySocket) {
        m_discoverySocket = new QUdpSocket(this);
        bool success = m_discoverySocket->bind(QHostAddress::AnyIPv4, 45000, QUdpSocket::ShareAddress);
        if (!success) {
            qWarning() << "[UDP] Failed to bind to port 45000 -" << m_discoverySocket->errorString();
            return;
        }

        connect(m_discoverySocket, &QUdpSocket::readyRead, this, &IviSocketServer::onDiscoveryRequest);
        qDebug() << "[UDP] Listening for requests on port 45000";
    }
}

void IviSocketServer::stopListening()
{
    if(m_clientSocket){
        m_clientSocket->disconnectFromHost();
        m_clientSocket->deleteLater();
        m_clientSocket = nullptr;
    }
    // m_server->close();
    if (m_server->isListening()) {
        m_server->close();
    }

    if (m_discoverySocket) {
        m_discoverySocket->close();
        m_discoverySocket->deleteLater();
        m_discoverySocket = nullptr;
    }

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
}

void IviSocketServer::onDisconnected()
{
    qDebug() << "Client disconnected. Waiting for new connection...";
    emit clientDisconnected();

    m_clientSocket->deleteLater();
    m_clientSocket = nullptr;
}

void IviSocketServer::onDiscoveryRequest()
{
    while (m_discoverySocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_discoverySocket->receiveDatagram();
        QString message = QString::fromUtf8(datagram.data()).trimmed();

        if (message == "DISCOVER_SERVER") {
            // QByteArray response = QString("%1:%2").arg(m_hostAddress).arg(m_port).toUtf8();
            QByteArray response = QString("SERVER:%1").arg(m_port).toUtf8();
            m_discoverySocket->writeDatagram(response, datagram.senderAddress(), datagram.senderPort());

            qDebug() << "[DISCOVERY] Responded to" << datagram.senderAddress() << ":" << datagram.senderPort()
                     << "→" << response;
        } else {
            qDebug() << "[DISCOVERY] Unknown message:" << message;
        }
    }
}

