#ifndef HMDEVICE_H
#define HMDEVICE_H

#include <QObject>



class HMDevice : public QObject
{
    Q_OBJECT
public:

    explicit HMDevice(QObject *parent = 0);

    enum HomematicType {
        HOMEMAITC,
        HOMEMATIC_IP
    };

    QString address() const;
    void setAddress(const QString &address);


    QString type() const;
    void setType(const QString &type);

    QString firmware() const;
    void setFirmware(const QString &firmware);

    QString gatewayId() const;
    void setGatewayId(const QString &gatewayId);

    QString homegearId() const;
    void setHomegearId(const QString &homegearId);

    QString availableFirmware() const;
    void setAvailableFirmware(const QString &availableFirmware);

    int deviceType() const;
    void setDeviceType(const int &deviceType);

    QString name() const;
    void setName(const QString &name);

signals:

private:
    QString m_address;
    QString m_name;
    QString m_type;
    QString m_firmware;
    QString m_availableFirmware;
    int  m_deviceType;
    QString m_gatewayId;
    QString m_homegearId;




};




#endif // HMDEVICE_H
