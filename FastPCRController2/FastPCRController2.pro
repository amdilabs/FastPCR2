QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../lib/crc/crc16.cpp \
    ../../lib/deflate.cpp \
    ../../lib/fifo.cpp \
    ../../lib/filenamevalidator.cpp \
    ../../lib/log.cpp \
    ../../lib/math/gaussian.cpp \
    ../../lib/math/matrix.cpp \
    ../../lib/math/regression.cpp \
    ../../lib/settings.cpp \
    ../../lib/signalingelapsedtimer.cpp \
    ../../lib/ui/customqcustomplot.cpp \
    ../../lib/ui/delegate/doublespinboxdelegate.cpp \
    ../../lib/ui/delegate/spinboxdelegate.cpp \
    ../../lib/ui/serialportitemmodel.cpp \
    ../../lib/ui/serialportselectiondialog.cpp \
    ../../lib/ui/userinput/userinputconfiguration.cpp \
    ../../lib/ui/userinput/userinputdata.cpp \
    ../../lib/ui/userinput/userinputwidgets.cpp \
    ../../lib/usb/usbdevice.cpp \
    ../../lib/usb/usbdeviceinfo.cpp \
    ../../lib/usb/usbinterrupt.cpp \
    ../../lib/usb/usbreader.cpp \
    ../cycler/fillingconfiguration.cpp \
    ../cycler/interface.cpp \
    ../cycler/message.cpp \
    ../cycler/meteringconfiguration.cpp \
    ../cycler/mixingconfiguration.cpp \
    ../cycler/pcrprotocol.cpp \
    ../cycler/pge.cpp \
    ../cycler/prehbhconfiguration.cpp \
    ../cycler/prehbhmixingconfiguration.cpp \
    ../cycler/resuspensionconfiguration.cpp \
    ../cycler/rtconfiguration.cpp \
    ../cycler/sampleprepconfiguration.cpp \
    ../cycler/slip.cpp \
    ../cycler/thermocyclingconfiguration.cpp \
    ../filewriter.cpp \
    ../induction/induction.cpp \
    ../io.cpp \
    ../optics/captureconfiguration.cpp \
    ../optics/captureitem.cpp \
    ../optics/cuvetteid.cpp \
    ../optics/cuvetteintensities.cpp \
    ../optics/cuvettelocationconfiguration.cpp \
    ../optics/cuvettelocationconfigurationcontainer.cpp \
    ../optics/cuvetteprocessingconfiguration.cpp \
    ../optics/ledconfiguration.cpp \
    ../optics/ledconfigurationcontainer.cpp \
    ../optics/opticsdevice.cpp \
    ../optics/pcrprofile.cpp \
    ../optics/rawdata.cpp \
    ../optics/spatialdata.cpp \
    ../optics/timedata.cpp \
    ../tags.cpp \
    ../tagwriter.cpp \
    ../ui/colourmap.cpp \
    ../ui/cuvettelocationconfigurationdialog.cpp \
    ../ui/cuvettelocationconfigurationtable.cpp \
    ../ui/cuvettelocationconfigurationtablemodel.cpp \
    ../ui/cuvetteprocessingconfigurationdialog.cpp \
    ../ui/pcrprofileplot.cpp \
    ../ui/spatialdataplot.cpp \
    ../ui/timedataplot.cpp \
    controller.cpp \
    cycler.cpp \
    diskconfiguration.cpp \
    diskconfigurationdialog.cpp \
    ledconfigurationdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    progress.cpp \
    progresstracker.cpp \
    scheduler.cpp \
    temperatureplot.cpp \
    valve.cpp

HEADERS += \
    ../../lib/crc/crc16.h \
    ../../lib/deflate.h \
    ../../lib/fifo.h \
    ../../lib/filenamevalidator.h \
    ../../lib/log.h \
    ../../lib/math/gaussian.h \
    ../../lib/math/matrix.h \
    ../../lib/math/regression.h \
    ../../lib/settings.h \
    ../../lib/signalingelapsedtimer.h \
    ../../lib/ui/customqcustomplot.h \
    ../../lib/ui/delegate/doublespinboxdelegate.h \
    ../../lib/ui/delegate/spinboxdelegate.h \
    ../../lib/ui/serialportitemmodel.h \
    ../../lib/ui/serialportselectiondialog.h \
    ../../lib/ui/userinput/userinputconfiguration.h \
    ../../lib/ui/userinput/userinputdata.h \
    ../../lib/ui/userinput/userinputwidgets.h \
    ../../lib/usb/usbdevice.h \
    ../../lib/usb/usbdeviceinfo.h \
    ../../lib/usb/usbinterrupt.h \
    ../../lib/usb/usbreader.h \
    ../cycler/fillingconfiguration.h \
    ../cycler/interface.h \
    ../cycler/message.h \
    ../cycler/meteringconfiguration.h \
    ../cycler/mixingconfiguration.h \
    ../cycler/pcrprotocol.h \
    ../cycler/preHbhmixingconfiguration.h \
    ../cycler/prehbhconfiguration.h \
    ../cycler/prehbhmixingconfiguration.h \
    ../cycler/resuspensionconfiguration.h \
    ../cycler/rtconfiguration.h \
    ../cycler/sampleprepconfiguration.h \
    ../cycler/slip.h \
    ../cycler/thermocyclingconfiguration.h \
    ../filewriter.h \
    ../induction/induction.h \
    ../io.h \
    ../optics/captureconfiguration.h \
    ../optics/captureitem.h \
    ../optics/cuvetteid.h \
    ../optics/cuvetteintensities.h \
    ../optics/cuvettelocationconfiguration.h \
    ../optics/cuvettelocationconfigurationcontainer.h \
    ../optics/cuvetteprocessingconfiguration.h \
    ../optics/ledconfiguration.h \
    ../optics/ledconfigurationcontainer.h \
    ../optics/opticsdevice.h \
    ../optics/pcrprofile.h \
    ../optics/rawdata.h \
    ../optics/spatialdata.h \
    ../optics/timedata.h \
    ../tags.h \
    ../tagwriter.h \
    ../ui/colourmap.h \
    ../ui/cuvettelocationconfigurationdialog.h \
    ../ui/cuvettelocationconfigurationtable.h \
    ../ui/cuvettelocationconfigurationtablemodel.h \
    ../ui/cuvetteprocessingconfigurationdialog.h \
    ../ui/pcrprofileplot.h \
    ../ui/spatialdataplot.h \
    ../ui/timedataplot.h \
    controller.h \
    cycler.h \
    diskconfiguration.h \
    diskconfigurationdialog.h \
    ledconfigurationdialog.h \
    mainwindow.h \
    progress.h \
    progresstracker.h \
    scheduler.h \
    temperatureplot.h \
    valve.h

FORMS += \
    ../../lib/ui/serialportselectiondialog.ui \
    ../ui/cuvettelocationconfigurationdialog.ui \
    ../ui/cuvetteprocessingconfigurationdialog.ui \
    diskconfigurationdialog.ui \
    ledconfigurationdialog.ui \
    mainwindow.ui

LIBS += -lusb-1.0 -lz -lqcustomplot -Wl,-rpath,/usr/local/lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Log_Ming07252023
