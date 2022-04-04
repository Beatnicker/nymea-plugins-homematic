#ifndef CCUCONNECTION_H
#define CCUCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>




class CcuConnection : public QObject
{
    Q_OBJECT
public:
    explicit CcuConnection(const QHostAddress &hostAddress, int port = 9090, QObject *parent = nullptr);


    void connectCcu();
    void disconnectCcu();

    QHostAddress hostAddress() const;
    int port() const;
    int httpPort() const;

    bool connected();

private:

    bool m_connected;
    int m_port;
    QHostAddress m_hostAddress;

    QNetworkAccessManager *m_NetMan;
    QNetworkReply *m_NetReply;

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void readData();

signals:
    void connectionStatusChanged();
    void dataReady(const QByteArray &data);

public slots:
    void sendData(const QByteArray &message);
};

#endif // CCUCONNECTION_H
