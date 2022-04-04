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

#ifndef INTEGRATIONPLUGINHOMEMATIC_H
#define INTEGRATIONPLUGINHOMEMATIC_H

#include "integrations/integrationplugin.h"
#include "ccu.h"
#include "plugintimer.h"

class IntegrationPluginHomematic: public IntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginHomematic.json")
    Q_INTERFACES(IntegrationPlugin)


public:
    explicit IntegrationPluginHomematic();

    void init() override;

    void setupThing(ThingSetupInfo *info) override;

    void postSetupThing(Thing *thing) override;

    void executeAction(ThingActionInfo *info) override;

    void thingRemoved(Thing *thing) override;

    void discoverThings(ThingDiscoveryInfo *info) override;

private:

    QHash<Ccu *, Thing *> m_ccu;
    PluginTimer *m_pluginTimer = nullptr;



private slots:
    void ccuConnectionStatusChanged(const bool &connected);

    void radiatorThermostatFound();
    void radiatorThermostatDataUpdated();
};

#endif // INTEGRATIONPLUGINHOMEMATIC_H
