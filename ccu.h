#ifndef CCU_H
#define CCU_H

#include <QDateTime>
#include <QHostAddress>
#include <QObject>
//#include "ccuconnection.h"

#include "Devices/radiatorthermostat.h"
#include "maia/maiaXmlRpcClient.h"
#include "Devices/hmdevice.h"

class Ccu : public QObject
{
    Q_OBJECT
public:
    explicit Ccu(const QHostAddress &hostAddress, int port, QObject *parent = nullptr);

    // cube data access functions
      QString serialNumber() const;
      void setSerialNumber(const QString &serialNumber);

      QByteArray rfAddress() const;
      void setRfAddress(const QByteArray &rfAddress);

      int firmware() const;
      void setFirmware(const int &firmware);

      QHostAddress hostAddress() const;
      void setHostAddress(const QHostAddress &hostAddress);

      uint port() const;
      void setPort(const uint &port);

      QDateTime cubeDateTime() const;
      void setCubeDateTime(const QDateTime &cubeDateTime);

      bool portalEnabeld() const;

      QList<RadiatorThermostat*> radiatorThermostatList() {
          return m_radiatorThermostatList;
      }



     // QList<Room*> roomList();


      bool sendData(QByteArray data);

      bool isConnected() {
          return m_Connected;
      }
      bool isInitialized();

      void listDevices();


      void connectCcu();
      void disconnectCcu();

      void update();
      bool setDeviceSetpointTemp(QString adress, float value);
      bool setDeviceSetpointTempManuMode(QString adress, float value);
      bool setDeviceAutoMode(QString adress,bool on);
public slots:
      void onConnectionStatusChanged();
signals:
      void ccuConnectionStatusChanged(bool connected);
      void radiatorThermostatFound();
      void radiatorThermostatDataUpdated();
private slots:

      void processResponse(QVariant &);

      void processResponseListDevice(QVariant &);
      void repsoneFaultListDevice(int, const QString &);

      void processDeviceDescription(QVariant &);
      void repsoneFaultDeviceDescription(int, const QString &);


      void handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);


      void onFinishReadDeviceName(QNetworkReply *rep);
      void processResponseSetpointTemp(QVariant &arg) {
          qDebug() <<  arg.toString() ;

      }
private:
      // cube data
      QString m_serialNumber;
      QByteArray m_rfAddress;
      int m_firmware;
      QHostAddress m_hostAddress;
      uint m_port;
      QDateTime m_cubeDateTime;
      bool m_portalEnabeld;
      bool m_Connected;

      MaiaXmlRpcClient *m_rpc;



      //CcuConnection *m_connection;


     /* QList<Room*> m_roomList;
      QList<WallThermostat*> m_wallThermostatList;*/

      QList<RadiatorThermostat*> m_radiatorThermostatList;
      bool isDevice(QString address, bool allowBidCos);

      bool m_cubeInitialized;

      void readDeviceNames();
};

#endif // CCU_H
