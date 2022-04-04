#include "radiatorthermostat.h"



RadiatorThermostat::RadiatorThermostat(QObject *parent) :
    HMDevice(parent)
{
}

bool RadiatorThermostat::lowBat() const
{
    return m_lowBat;
}

float RadiatorThermostat::actualTemperature() const
{
    return m_actualTemperature;
}

void RadiatorThermostat::setActualTemperature(float actualTemperature)
{
    m_actualTemperature = actualTemperature;
}

uint8_t RadiatorThermostat::valveState() const
{
    return m_valveState;
}

float RadiatorThermostat::TargetTemperature() const
{
    return m_Temperature;
}

void RadiatorThermostat::setTargetTemperature(float Temperature)
{
    m_Temperature = Temperature;
}

void RadiatorThermostat::update(MaiaXmlRpcClient *&m_rpc) {


    QVariantList args;
    args.append(this->address()+":4");
    args.append("ACTUAL_TEMPERATURE");


    m_rpc->call("getValue", args,
                this, SLOT(processUpdateRadiatorThermostatTemp(QVariant&)),
                this, SLOT(repsoneFaultListDevice(int,const QString&)));


    args.clear();
    args.append(this->address()+":4");
    args.append("SET_TEMPERATURE");


    m_rpc->call("getValue", args,
                this, SLOT(processUpdateRadiatorThermostatTargetTemp(QVariant&)),
                this, SLOT(repsoneFaultListDevice(int,const QString&)));


    args.clear();
    args.append(this->address()+":4");
    args.append("VALVE_STATE");


    m_rpc->call("getValue", args,
                this, SLOT(processUpdateRadiatorThermostatValveState(QVariant&)),
                this, SLOT(repsoneFaultListDevice(int,const QString&)));






}
void RadiatorThermostat::processUpdateRadiatorThermostatTemp(QVariant &arg) {

    // do something with the arg
    qDebug() << arg.toString();
    float temperature = arg.toFloat();

    try {
      this->setActualTemperature(temperature );
    }catch(...) {
      this->setActualTemperature((float)0.0);
    }


}

void RadiatorThermostat::processUpdateRadiatorThermostatValveState(QVariant &arg) {

    // do something with the arg
    qDebug() << arg.toString();
    float valveState = arg.toInt();

    try {
      this->setValveState(valveState);
    }catch(...) {
        this->setValveState(0);
    }


    emit readAllDataDone();
}
void RadiatorThermostat::processUpdateRadiatorThermostatTargetTemp(QVariant &arg) {

    // do something with the arg
    qDebug() << arg.toString();
    float temperature = arg.toFloat();

    try {
      this->setTargetTemperature(temperature);
    }catch(...) {
      this->setActualTemperature((float)0.0);
    }


    emit readAllDataDone();
}

void RadiatorThermostat::setValveState(const uint8_t &valveState)
{
    m_valveState = valveState;
}

void RadiatorThermostat::repsoneFaultListDevice(int error, const QString &message) {
    qDebug() << "An Error occoured, Code: " << error << " Message: " << message;

}
