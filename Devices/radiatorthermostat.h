#ifndef RADIATORTHERMOSTAT_H
#define RADIATORTHERMOSTAT_H

#include <QObject>
#include <QMutex>
#include "hmdevice.h"
#include "maia/maiaXmlRpcClient.h"

class RadiatorThermostat : public HMDevice
{
    Q_OBJECT

public:
    explicit RadiatorThermostat(QObject *parent = 0);

    bool lowBat() const;

    float actualTemperature() const;

    void setActualTemperature(float actualTemperature);

    uint8_t valveState() const;

    float TargetTemperature() const;
    void setTargetTemperature(float TargetTemperature);



    void update(MaiaXmlRpcClient *&m_rpc);
    void setValveState(const uint8_t &valveState);

signals:
    void readAllDataDone();

private slots:
    void processUpdateRadiatorThermostatTemp(QVariant &arg);
    void repsoneFaultListDevice(int error, const QString &message);
    void processUpdateRadiatorThermostatTargetTemp(QVariant &arg);
    void processUpdateRadiatorThermostatValveState(QVariant &arg);
private:
    bool m_lowBat;
    float m_actualTemperature;
    uint8_t m_valveState;
    float m_Temperature;
    QMutex mutex;



};

#endif // RADIATORTHERMOSTAT_H
