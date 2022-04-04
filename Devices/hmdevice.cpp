#include "hmdevice.h"

HMDevice::HMDevice(QObject *parent) :
    QObject(parent)
{
}


QString HMDevice::address() const
{
    return m_address;
}

void HMDevice::setAddress(const QString &address)
{
    m_address = address;
}



QString HMDevice::type() const
{
    return m_type;
}

void HMDevice::setType(const QString &type)
{
    m_type = type;
}

QString HMDevice::firmware() const
{
    return m_firmware;
}

void HMDevice::setFirmware(const QString &firmware)
{
    m_firmware = firmware;
}

QString HMDevice::gatewayId() const
{
    return m_gatewayId;
}

void HMDevice::setGatewayId(const QString &gatewayId)
{
    m_gatewayId = gatewayId;
}

QString HMDevice::homegearId() const
{
    return m_homegearId;
}

void HMDevice::setHomegearId(const QString &homegearId)
{
    m_homegearId = homegearId;
}

QString HMDevice::availableFirmware() const
{
    return m_availableFirmware;
}

void HMDevice::setAvailableFirmware(const QString &availableFirmware)
{
    m_availableFirmware = availableFirmware;
}

int HMDevice::deviceType() const
{
    return m_deviceType;
}

void HMDevice::setDeviceType(const int &deviceType)
{
    m_deviceType = deviceType;
}

QString HMDevice::name() const
{
    return m_name;
}

void HMDevice::setName(const QString &name)
{
    m_name = name;
}





