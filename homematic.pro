include($$[QT_INSTALL_PREFIX]/include/nymea/plugin.pri)

INCLUDEPATH += /home/beatnicker/github.com/libmaia
LIBS += /home/beatnicker/github.com/libmaia/libmaia.a


QT += network xml

SOURCES += \
    Devices/hmdevice.cpp \
    Devices/radiatorthermostat.cpp \
    ccu.cpp \
    ccudiscovery.cpp \
    integrationpluginHomematic.cpp

HEADERS += \
    Devices/hmdevice.h \
    Devices/radiatorthermostat.h \
    ccu.h \
    ccudiscovery.h \
    integrationpluginHomematic.h

