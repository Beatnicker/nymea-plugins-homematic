#include "ccuconnection.h"

#include "extern-plugininfo.h"

#include "loggingcategories.h"



#include <QEventLoop>
#include <QNetworkAccessManager>
NYMEA_LOGGING_CATEGORY(dcCcu, "Homematic")


CcuConnection::CcuConnection(const QHostAddress &hostAddress, int port, QObject *parent) :
    QObject(parent),
    m_hostAddress(hostAddress),
    m_port(port),
    m_connected(false),
    m_NetMan(new QNetworkAccessManager(this)),
    m_NetReply(nullptr)

{
    QUrl url;
    url.setUrl( (QString) ("http://"+hostAddress+":"+QString(port)) );

   /* connect(m_networkManager, &QNetworkAccessManager::networkSessionConnected, this, &CcuConnection::onConnected);
    connect(m_networkManager, &QNetworkAccessManager::networkAccessibleChanged, this, &CcuConnection::onDisconnected);
    connect(m_networkManager, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
    connect(m_networkManager, &QNetworkAccessManager::readyRead, this, &CcuConnection::readData);*/
}

void CcuConnection::connectCcu()
{
   /* if (m_networkManager-> ->state() == QAbstractSocket::ConnectingState) {
        return;
    }*/

    qCInfo(dcHomematic()) << "Host Adress  " <<  m_hostAddress << ":" << m_port ;
    m_networkManager->connectToHost(m_hostAddress, m_port);

}

void CcuConnection::disconnectCcu()
{
    m_socket->close();

}

QHostAddress CcuConnection::hostAddress() const
{
    return m_hostAddress;
}

int CcuConnection::port() const
{
    return m_port;
}

bool CcuConnection::connected()
{
    return m_connected;
}

void CcuConnection::onConnected()
{

    qCInfo(dcHomematic())  << "connected successfully to" << hostAddress().toString() << port();
    m_connected = true;
    emit connectionStatusChanged();
}

void CcuConnection::onDisconnected()
{
    qCDebug(dcCcu) << "disconnected from" << hostAddress().toString() << port();
    m_networkManager-
    m_connected = false;
    emit connectionStatusChanged();
}

void CcuConnection::onError(QAbstractSocket::SocketError socketError)
{
    if (connected()) {
        qCWarning(dcCcu) << "socket error:" << socketError << m_socket->errorString();
    }
}

void CcuConnection::readData()
{
    QByteArray message;

    message = m_socket->readAll();

    emit dataReady(message);

}

void CcuConnection::sendData(const QByteArray &message)
{
    QNetworkReply *reply = m_networkAccessManager->post(message);


    connect(reply, &QNetworkReply::finished, this, [requestId, reply, this] {


       }

     m_socket->write(message);

}
