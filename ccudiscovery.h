#ifndef CCUDISCOVERY_H
#define CCUDISCOVERY_H

#include <QObject>

#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>


class CcuDiscovery : public QObject
{
    Q_OBJECT
public:

    class CcuInfo {
        public:
            QString serialNumber;
            QHostAddress hostAddress;
            int port = 0;
            QByteArray rfAddress;
            int firmware;
        };

        explicit CcuDiscovery(QObject *parent = nullptr);

        void detectCcus();

    private slots:
        void readData();
        void discoverTimeout();

    signals:
        void ccusDetected(const QList<CcuInfo> &ccuList);

    private:
        QUdpSocket *m_udpSocket;
        QTimer *m_timeout;

        quint16 m_port;

        QList<CcuInfo> m_CcuList;

};

#endif // CCUDISCOVERY_H
