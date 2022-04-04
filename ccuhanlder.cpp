#include "ccuhanlder.h"
#include <QtXml>
#include <iostream>
#include "Devices/hmdevice.h"

CcuHanlder::CcuHanlder(const QHostAddress &hostAddress, int port,QObject *parent) :
    QObject(parent),     
    m_id(0)
{

    QString test = "http://"+ hostAddress.toString()+ ":"+ QString::number(port) ;

     std::cout << test.toStdString() << std::endl;
    m_rpc = new MaiaXmlRpcClient(QUrl("http://"+ hostAddress.toString()+ ":" +  QString::number(port)) , this);


    QSslConfiguration config = m_rpc->sslConfiguration();
    config.setProtocol(QSsl::AnyProtocol);
    m_rpc->setSslConfiguration(config);

   connect(m_rpc, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
                this, SLOT(handleSslErrors(QNetworkReply *, const QList<QSslError> &)));

}

int CcuHanlder::listDevices()
{


    QVariantList args;
    m_rpc->call("listDevices", args,
                    this, SLOT(processResponseListDevice(QVariant&)),
                    this, SLOT(repsoneFaultListDevice(int,const QString&)));


    return 0;
}

