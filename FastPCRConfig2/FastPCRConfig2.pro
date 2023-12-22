QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../cycler/fillingconfiguration.cpp \
    ../cycler/meteringconfiguration.cpp \
    ../cycler/mixingconfiguration.cpp \
    ../cycler/pcrprotocol.cpp \
    ../cycler/preHbhMixingconfiguration.cpp \
    ../cycler/prehbhconfiguration.cpp \
    ../cycler/resuspensionconfiguration.cpp \
    ../cycler/rtconfiguration.cpp \
    ../cycler/sampleprepconfiguration.cpp \
    ../cycler/thermocyclingconfiguration.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ../cycler/fillingconfiguration.h \
    ../cycler/meteringconfiguration.h \
    ../cycler/mixingconfiguration.h \
    ../cycler/pcrprotocol.h \
    ../cycler/preHbhMixingconfiguration.h \
    ../cycler/prehbhconfiguration.h \
    ../cycler/resuspensionconfiguration.h \
    ../cycler/rtconfiguration.h \
    ../cycler/sampleprepconfiguration.h \
    ../cycler/thermocyclingconfiguration.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
