QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../lib/inflate.cpp \
    ../../lib/math/gaussian.cpp \
    ../../lib/math/matrix.cpp \
    ../../lib/math/regression.cpp \
    ../../lib/settings.cpp \
    ../../lib/ui/customqcustomplot.cpp \
    ../../lib/ui/delegate/doublespinboxdelegate.cpp \
    ../../lib/ui/delegate/spinboxdelegate.cpp \
    ../io.cpp \
    ../optics/captureconfiguration.cpp \
    ../optics/captureitem.cpp \
    ../optics/ct.cpp \
    ../optics/ctconfiguration.cpp \
    ../optics/cuvetteid.cpp \
    ../optics/cuvetteintensities.cpp \
    ../optics/cuvettelocationconfiguration.cpp \
    ../optics/cuvettelocationconfigurationcontainer.cpp \
    ../optics/cuvetteprocessingconfiguration.cpp \
    ../optics/opticsconfiguration.cpp \
    ../optics/pcrprofile.cpp \
    ../optics/pcrvalues.cpp \
    ../optics/rawdata.cpp \
    ../optics/spatialdata.cpp \
    ../optics/timedata.cpp \
    ../tagreader.cpp \
    ../ui/colourmap.cpp \
    ../ui/cuvettelocationconfigurationdialog.cpp \
    ../ui/cuvettelocationconfigurationtable.cpp \
    ../ui/cuvettelocationconfigurationtablemodel.cpp \
    ../ui/cuvetteprocessingconfigurationdialog.cpp \
    ../ui/pcrprofileplot.cpp \
    ../ui/pcrtable.cpp \
    ../ui/pcrtablemodel.cpp \
    ../ui/plotconfiguration.cpp \
    ../ui/spatialdataplot.cpp \
    ../ui/timedataplot.cpp \
    bardelegate.cpp \
    ctconfigurationdialog.cpp \
    cttable.cpp \
    cttablemodel.cpp \
    filereader.cpp \
    main.cpp \
    mainwindow.cpp \
    pcrfile.cpp \
    smoothnpplot.cpp \
    temperaturedata.cpp \
    temperatureplot.cpp

HEADERS += \
    ../../lib/inflate.h \
    ../../lib/math/gaussian.h \
    ../../lib/math/matrix.h \
    ../../lib/math/regression.h \
    ../../lib/settings.h \
    ../../lib/ui/customqcustomplot.h \
    ../../lib/ui/delegate/doublespinboxdelegate.h \
    ../../lib/ui/delegate/spinboxdelegate.h \
    ../io.h \
    ../optics/captureconfiguration.h \
    ../optics/captureitem.h \
    ../optics/ct.h \
    ../optics/ctconfiguration.h \
    ../optics/cuvetteid.h \
    ../optics/cuvetteintensities.h \
    ../optics/cuvettelocationconfiguration.h \
    ../optics/cuvettelocationconfigurationcontainer.h \
    ../optics/cuvetteprocessingconfiguration.h \
    ../optics/opticsconfiguration.h \
    ../optics/pcrprofile.h \
    ../optics/pcrvalues.h \
    ../optics/rawdata.h \
    ../optics/spatialdata.h \
    ../optics/timedata.h \
    ../tagreader.h \
    ../ui/colourmap.h \
    ../ui/cuvettelocationconfigurationdialog.h \
    ../ui/cuvettelocationconfigurationtable.h \
    ../ui/cuvettelocationconfigurationtablemodel.h \
    ../ui/cuvetteprocessingconfigurationdialog.h \
    ../ui/pcrprofileplot.h \
    ../ui/pcrtable.h \
    ../ui/pcrtablemodel.h \
    ../ui/plotconfiguration.h \
    ../ui/spatialdataplot.h \
    ../ui/timedataplot.h \
    bardelegate.h \
    ctconfigurationdialog.h \
    cttable.h \
    cttablemodel.h \
    filereader.h \
    mainwindow.h \
    pcrfile.h \
    smoothnpplot.h \
    temperaturedata.h \
    temperatureplot.h

FORMS += \
    ../ui/cuvettelocationconfigurationdialog.ui \
    ../ui/cuvetteprocessingconfigurationdialog.ui \
    ctconfigurationdialog.ui \
    mainwindow.ui

LIBS += -lz -lqcustomplot -Wl,-rpath,/usr/local/lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
