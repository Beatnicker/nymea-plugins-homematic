/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Copyright (C) 2020 Michael Menner <m.menner@t-online.de>                 *
 *                                                                         *
 *  This library is free software; you can redistribute it and/or          *
 *  modify it under the terms of the GNU Lesser General Public             *
 *  License as published by the Free Software Foundation;                  *
 *  version 3 of the License.                                              *
 *                                                                         *
 *  This library is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
 *  Lesser General Public License for more details.                        *
 *                                                                         *
 *  You should have received a copy of the GNU Lesser General Public       *
 *  License along with this library; If not, see                           *
 *  <http://www.gnu.org/licenses/>.                                        *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "plugininfo.h"
#include "integrationpluginHomematic.h"
#include "ccudiscovery.h"

#include "ccu.h"

IntegrationPluginHomematic::IntegrationPluginHomematic()
{

}

void IntegrationPluginHomematic::init()
{
    // Initialisation can be done here.
    qCDebug(dcHomematic()) << " IntegrationPluginHomematic Plugin initialized.";




}


void IntegrationPluginHomematic::setupThing(ThingSetupInfo *info)
{
    // A thing is being set up. Use info->thing() to get details of the thing, do
    // the required setup (e.g. connect to the device) and call info->finish() when done.

    qCInfo(dcHomematic()) << "Setup thing" << info->thing();


    qCInfo(dcHomematic()) << "Bal blu blup " ;

    info->finish(Thing::ThingErrorNoError);

    Thing *thing = info->thing();

    if(thing->thingClassId() == ccuThingClassId){

        Ccu *ccu = new Ccu((QHostAddress) thing->paramValue(ccuThingHostParamTypeId).toString(),thing->paramValue(ccuThingPortParamTypeId).toUInt(),this);
        connect(ccu, &Ccu::ccuConnectionStatusChanged, this, &IntegrationPluginHomematic::ccuConnectionStatusChanged);
        ccu->listDevices();
        m_ccu.insert(ccu,thing);

        connect(ccu, &Ccu::radiatorThermostatFound, this, &IntegrationPluginHomematic::radiatorThermostatFound);
        connect(ccu, &Ccu::radiatorThermostatDataUpdated, this, &IntegrationPluginHomematic::radiatorThermostatDataUpdated);
        //connect(CCu, &Ccu::ccuConnectionStatusChanged)

      //  ccu->connectCcu();

    }

}

void IntegrationPluginHomematic::postSetupThing(Thing *thing)
{
    qCDebug(dcHomematic()) << "Post setup Thing" << thing;
    if (thing->thingClassId() == ccuThingClassId) {
        if (!m_pluginTimer) {
            qCDebug(dcHomematic()) << "Starting plugin timer...";
            m_pluginTimer = hardwareManager()->pluginTimerManager()->registerTimer(10);
            connect(m_pluginTimer, &PluginTimer::timeout, this, [this] {
                foreach (Ccu *ccu, m_ccu.keys()) {
                    if (ccu->isConnected()) {
                        ccu->update();
                    }
                }
            });

            m_pluginTimer->start();
        }
    }

}

void IntegrationPluginHomematic::executeAction(ThingActionInfo *info)
{
    // An action is being executed. Use info->action() to get details about the action,
    // do the required operations (e.g. send a command to the network) and call info->finish() when done.

    Action action = info->action();

    qCInfo(dcHomematic()) << "Executing action for thing" << info->thing() << info->action().actionTypeId().toString() << info->action().params();
    if (info->thing()->thingClassId() == radiatorThermostatThingClassId){
        Thing *thing = myThings().findById(info->thing()->parentId());
        Ccu *ccu = m_ccu.key(thing);
        QString address  = info->thing()->paramValue(radiatorThermostatThingAdressParamTypeId).toString();

        //int id = -1;
        if (action.actionTypeId() == radiatorThermostatTargetTemperatureActionTypeId){
            float temperature = action.param(radiatorThermostatTargetTemperatureActionTargetTemperatureParamTypeId).value().toFloat();
            ccu->setDeviceSetpointTemp(address, temperature);

        }
        if (action.actionTypeId() == radiatorThermostatSetManualModeActionTypeId) {
            float temperature = action.param(radiatorThermostatTargetTemperatureActionTargetTemperatureParamTypeId).value().toFloat();
            ccu->setDeviceSetpointTempManuMode(address, temperature);
        }
        if (action.actionTypeId() == radiatorThermostatSetAutoModeActionTypeId) {

            ccu->setDeviceAutoMode(address,true);
        }

        if(action.actionTypeId() == radiatorThermostatPowerActionTypeId){
            if (action.param(radiatorThermostatPowerActionPowerParamTypeId).value().toBool() == true) {
                 ccu->setDeviceSetpointTempManuMode(address, 20);
            } else {
                ccu->setDeviceSetpointTempManuMode(address, 4.5);
            }
        }




    }

    info->finish(Thing::ThingErrorNoError);
}

void IntegrationPluginHomematic::thingRemoved(Thing *thing)
{
    // A thing is being removed from the system. Do the required cleanup
    // (e.g. disconnect from the device) here.

    qCInfo(dcHomematic()) << "Remove thing" << thing;


}

void IntegrationPluginHomematic::ccuConnectionStatusChanged(const bool &connected)
{
    Ccu *ccu = static_cast<Ccu*>(sender());
    Thing *thing;
     qCInfo(dcHomematic()) << "ccuConnectionStatusChanged " << connected;
    if (connected) {
        if (m_ccu.contains(ccu)) {
            thing = m_ccu.value(ccu);
            //thing->setName("CCU " + ccu->serialNumber());
            thing->setStateValue(ccuConnectedStateTypeId,true);
            ccu->listDevices();
        }
    } else {
        if (m_ccu.contains(ccu)){
            thing = m_ccu.value(ccu);
            thing->setStateValue(ccuConnectedStateTypeId,false);
        }
    }
}
void IntegrationPluginHomematic::discoverThings(ThingDiscoveryInfo *info)
{
  //  ThingClassId deviceClassId = info->thingClassId();
    qCDebug(dcHomematic()) << "Discover devices called" << info;

}


void IntegrationPluginHomematic::radiatorThermostatFound() {
    Ccu *ccu  = static_cast<Ccu*>(sender());

    QList<ThingDescriptor> descriptorList;
    foreach (RadiatorThermostat *radiatorThermostat, ccu->radiatorThermostatList()) {
            bool alreadyAdded = false;
            foreach (Thing *thing, myThings().filterByThingClassId(radiatorThermostatThingClassId)){
                if(radiatorThermostat->address() == thing->paramValue(radiatorThermostatThingAdressParamTypeId).toString()){
                    alreadyAdded = true;
                    break;
                }
            }
            if(!alreadyAdded){
                ThingDescriptor descriptor(radiatorThermostatThingClassId, "HT " + radiatorThermostat->name() + "");
                descriptor.setParentId(m_ccu.value(ccu)->id());
                ParamList params;
                params.append(Param(radiatorThermostatThingAdressParamTypeId, radiatorThermostat->address()));
                 params.append(Param(radiatorThermostatThingNameParamTypeId, radiatorThermostat->name()));

                descriptor.setParams(params);
                descriptorList.append(descriptor);
            }
        }

        if(!descriptorList.isEmpty()){
            emit autoThingsAppeared(descriptorList);
        }
}
void IntegrationPluginHomematic::radiatorThermostatDataUpdated() {
    Ccu *ccu  = static_cast<Ccu*>(sender());

    foreach (RadiatorThermostat *radiatorThermostat, ccu->radiatorThermostatList()) {
            foreach (Thing *thing, myThings().filterByThingClassId(radiatorThermostatThingClassId)){
                if(thing->paramValue(radiatorThermostatThingAdressParamTypeId).toString() == radiatorThermostat->address()){

                    thing->setStateValue(radiatorThermostatConnectedStateTypeId, true);
                    thing->setStateValue(radiatorThermostatTemperatureStateTypeId, radiatorThermostat->actualTemperature());
                    thing->setStateValue(radiatorThermostatTargetTemperatureStateTypeId, radiatorThermostat->TargetTemperature());
                    thing->setStateValue(radiatorThermostatValvePositionStateTypeId, radiatorThermostat->valveState());

                }
            }
        }

}


