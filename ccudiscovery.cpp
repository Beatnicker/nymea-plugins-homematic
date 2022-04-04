#include "ccudiscovery.h"

#include "extern-plugininfo.h"



CcuDiscovery::CcuDiscovery(QObject *parent) :
    QObject(parent)
{
    // UDP broadcast for cube detection in the network
    m_udpSocket = new QUdpSocket(this);
    m_port = 43439;
    m_udpSocket->bind(m_port,QUdpSocket::ShareAddress);

    m_timeout = new QTimer(this);
    m_timeout->setSingleShot(true);

    connect(m_udpSocket,SIGNAL(readyRead()),this,SLOT(readData()));
    connect(m_timeout,SIGNAL(timeout()),this,SLOT(discoverTimeout()));
}

void CcuDiscovery::detectCcus()
{
    m_CcuList.clear();

    QByteArray byteArray;

    int senderId = 452 & 0xFFFFFF;
    byteArray.append(2);
    for (int i = 2; i >= 0; i--) {
        char temp = (char) (senderId >> i * 8 & 0xFF);
                byteArray.append(temp);
    }
    byteArray.append(0x73);
    byteArray.append("0");
    byteArray.append(QString("eQ3-*").toUtf8()) ;
    byteArray.append("0");
    byteArray.append(QString("*").toUtf8()) ;
    byteArray.append("0");
    byteArray.append(0x73);
    // broadcast the hello message, every cube should respond with a 26 byte message
    m_udpSocket->writeDatagram(byteArray, QHostAddress::Broadcast, m_port);
    m_timeout->start(1500);
}

void CcuDiscovery::readData()
{
    QByteArray data;
    QHostAddress sender;
    quint16 udpPort;

    // read the answere from the
    while (m_udpSocket->hasPendingDatagrams()) {
        data.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(data.data(), data.size(), &sender, &udpPort);
    }
    if(!data.isEmpty() && data.contains("eQ3")){

        CcuInfo ccu;
        ccu.hostAddress = sender;
        ccu.serialNumber = data.mid(8,10);
        ccu.rfAddress = data.mid(21,3).toHex();
        ccu.firmware = data.mid(24,2).toHex().toInt();

        // set port depending on the firmware
        if(ccu.firmware < 109){
            ccu.port= 80;
        }else{
            ccu.port = 62910;
        }

        m_CcuList.append(ccu);
    }
}

void CcuDiscovery::discoverTimeout()
{
    emit ccusDetected(m_CcuList);
}
