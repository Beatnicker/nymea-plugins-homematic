#include "ccu.h"
#include "extern-plugininfo.h"
#include <QMap>
#include <QList>
#include <QtXml>
# include <iostream>




Ccu::Ccu(const QHostAddress &hostAddress, int port, QObject *parent) :
    QObject(parent),
    m_hostAddress(hostAddress)
{

   // m_connection = new CcuConnection(hostAddress, port, this);
   // connect (m_connection, &CcuConnection::connectionStatusChanged, this, &Ccu::onConnectionStatusChanged);


/*    connect(m_CcuHandler, &CcuHanlder::notificationReceived, this, &Ccu::processNotification);
    connect(m_CcuHandler, &CcuHanlder::replyReceived, this, &Ccu::processResponse);*/


    m_rpc = new MaiaXmlRpcClient(QUrl("http://"+ hostAddress.toString()+ ":" +  QString::number(port)) , this);


    QSslConfiguration config = m_rpc->sslConfiguration();
    config.setProtocol(QSsl::AnyProtocol);
    m_rpc->setSslConfiguration(config);

   connect(m_rpc, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
                this, SLOT(handleSslErrors(QNetworkReply *, const QList<QSslError> &)));




}

void Ccu::listDevices()
{
    QVariantList args;
    m_rpc->call("listDevices", args,
                    this, SLOT(processResponseListDevice(QVariant&)),
                    this, SLOT(repsoneFaultListDevice(int,const QString&)));



}

void Ccu::processResponseListDevice(QVariant &arg) {
   // do something with the arg
    qDebug() << arg.toString();
    QList<QVariant> myDevices = arg.toList();

    foreach ( const QVariant var , myDevices) {

        QMap<QString, QVariant> items = var.toMap();
        if(isDevice(items["ADDRESS"].toString(),false)) {
            QString address = items["ADDRESS"].toString();
            QString type = items["TYPE"].toString();
            QString firmware = items["FIRMWARE"].toString();
            QString availableFirmware = items["AVAILABLE_FIRMWARE"].toString();
            std::cout << address.toStdString() <<  " " <<  " "  << type.toStdString() << " " << firmware.toStdString() << std::endl;



            if(type == "HM-CC-RT-DN") {
                 RadiatorThermostat* thing = new RadiatorThermostat(this);
                 thing->setAddress(address);
                 thing->setType(type);
                 thing->setFirmware(firmware);
                 thing->setAvailableFirmware(availableFirmware);
                 thing->setDeviceType(HMDevice::HomematicType::HOMEMAITC);
                 m_radiatorThermostatList.append(thing);
                 QVariantList args2;
                 args2.append(address );
            }

        }
    }

    m_Connected = true;

    readDeviceNames();

}

void Ccu::readDeviceNames(){

    QUrl url = QUrl("http://"+ m_hostAddress.toString()+ ":8181/tclrega.exe") ;
    QNetworkRequest request(url);

    QString postData = "string dev_id; \
            Write('<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>\n'); \
            Write(\"<list>\n\"); \
            foreach (dev_id, root.Devices().EnumUsedIDs()) { \
              object dev_obj = dom.GetObject(dev_id); \
              Write(\"  <entry>\"); \
              Write(\"    <adress>\"); WriteXML(dev_obj.Address()); Write(\"</adress>\"); \
              Write(\"    <name>\"); WriteXML(dev_obj.Name()); Write(\"</name>\");  \
              Write(\"  </entry>\n\"); \
            } \
            Write(\"</list>\"); ";

    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinishReadDeviceName(QNetworkReply*)));
    connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));


    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/xml");

    mgr->post(QNetworkRequest(url), postData.toUtf8());

}

void Ccu::onFinishReadDeviceName(QNetworkReply *rep)
{
    QByteArray array = rep->readAll();
    QString DataAsString = QString(array);

    int lastIndex = DataAsString.lastIndexOf("<xml><exec>");
    if(lastIndex != 0 ) {
        DataAsString = DataAsString.left(lastIndex);
    } else {
        return;
    }
    std::cout  << DataAsString.toStdString() << std::endl;
    qDebug() << DataAsString;

    QXmlStreamReader reader(DataAsString);
    QString adress = "";
    QString name =  "";
    if (reader.readNextStartElement()) {
        if (reader.name() == "list"){
            while(reader.readNextStartElement()){
                if(reader.name() == "entry"){
                    while(reader.readNextStartElement()){
                        if(reader.name() == "adress"){
                            adress = reader.readElementText();
                        }
                        else if(reader.name() == "name"){
                            name = reader.readElementText();
                        }
                        else
                            reader.skipCurrentElement();

                    }
                    foreach (RadiatorThermostat *thing, m_radiatorThermostatList) {
                        if(thing->address().contains(adress)) {
                            thing->setName(name);
                        }

                    }
                }
                else
                    reader.skipCurrentElement();
            }
        }
        else
            reader.raiseError(QObject::tr("Incorrect file"));
    }

    if(!m_radiatorThermostatList.isEmpty()) {
        emit radiatorThermostatFound();
    }

}


void Ccu::update() {



    //update radiator Thermostats
    foreach(RadiatorThermostat *thing, m_radiatorThermostatList){
        connect(thing, &RadiatorThermostat::readAllDataDone,this, [this] {
            emit radiatorThermostatDataUpdated();
        });
        thing->update(m_rpc);
    }


}

/**
 * Returns true if address is a device. If allowBidCos ist true then addresses starting with "BidCos" classified as
 * devices, too.
 */
bool Ccu::isDevice(QString address, bool allowBidCos) {
    if (address == NULL) {
        return false;
    }
    if (address.contains(":")) {
        return false;
    }
    if (allowBidCos && address.startsWith("BidCos")) {
        return true;
    }
    return !address.startsWith("BidCos");
}




void Ccu::processDeviceDescription(QVariant &arg) {
   // do something with the arg
    qDebug() << arg.toString();
}

bool Ccu::setDeviceSetpointTemp(QString adress, float value) {
    QVariantList args;
    args.append(adress+":4");
    args.append("SET_TEMPERATURE");
    args.append(QString::number(value));
    m_rpc->call("setValue", args,
                    this, SLOT(processResponse(QVariant&)),
                    this, SLOT(repsoneFaultListDevice(int,const QString&)));

    return true;
}

bool Ccu::setDeviceSetpointTempManuMode(QString adress, float value) {
    QVariantList args;
    args.append(adress+":4");
    args.append("MANU_MODE");
    args.append(QString::number(value));
    m_rpc->call("setValue", args,
                    this, SLOT(processResponse(QVariant&)),
                    this, SLOT(repsoneFaultListDevice(int,const QString&)));

    return true;
}


bool Ccu::setDeviceAutoMode(QString adress,bool on) {
    QVariantList args;
    args.append(adress+":4");
    args.append("AUTO_MODE");
    args.append(on);
    m_rpc->call("setValue", args,
                    this, SLOT(processResponse(QVariant&)),
                    this, SLOT(repsoneFaultListDevice(int,const QString&)));

    return true;
}



void Ccu::repsoneFaultListDevice(int error, const QString &message) {
   qDebug() << "An Error occoured, Code: " << error << " Message: " << message;

}
void Ccu::repsoneFaultDeviceDescription(int error, const QString &message) {
   qDebug() << "An Error occoured, Code: " << error << " Message: " << message;
}

void Ccu::handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
    qDebug() << "SSL Error:" << errors;
    reply->ignoreSslErrors(); // don't do this in real code! Fix your certs!
}

void Ccu::processResponse(QVariant &arg)
{
  qDebug() <<  arg.toString() ;
}

void Ccu::onConnectionStatusChanged()
{
    qCInfo(dcHomematic()) << "onConnectionStatusChanged in cccu   " ;

}
void Ccu::connectCcu()
{
    m_Connected = true;
}

void Ccu::disconnectCcu()
{
m_Connected = false;
}



