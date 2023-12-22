#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCheckBox>
#include <QMainWindow>
#include "../optics/ct.h"
#include "../optics/pcrvalues.h"
#include "../optics/ctconfiguration.h"
#include "../optics/cuvettelocationconfigurationcontainer.h"
#include "../optics/cuvetteprocessingconfiguration.h"
#include "../optics/pcrprofile.h"
#include "pcrfile.h"

namespace Ui
{ class MainWindow;
}

class MainWindow : public QMainWindow
{ Q_OBJECT

  public:
    MainWindow (QWidget *parent = nullptr);
    ~MainWindow (void);

  private slots:
    void actionFileOpenTriggered (void);
    void actionFileExportTimeDataTriggered (void);
    void actionFileExportSpatialDataTriggered (void);
    void actionFileExportCtTriggered (void);
    void actionFileExportProfileTriggered (void);
    void actionConfigurationCuvetteTriggered (void);
    void actionConfigurationProcessingTriggered (void);
    void actionConfigurationCtTriggered (void);
    void pcrGraphChannelBoxClicked (void);
    void pcrGraphCuvetteBoxClicked (void);
    void smoothNpChannelBoxClicked (void);
    void smoothNpCuvetteBoxClicked (void);
    void timeDataChannelBoxClicked (void);
    void timeDataCycleBoxEditingFinished (void);
    void spatialDataChannelBoxClicked (void);
    void spatialDataCycleBoxEditingFinished (void);

  private:
    Ui::MainWindow *ui;
    PcrFile m_file;
    QString m_fileName;
    CuvetteLocationConfigurationContainer m_cuvetteConfig;
    CuvetteProcessingConfiguration m_processingConfig;
    CtConfiguration m_ctConfig;
    PcrValues m_values;

    QList<QCheckBox *> m_pcrGraphChannelBoxes;
    QList<QCheckBox *> m_pcrGraphCuvetteBoxes;
    QList<QCheckBox *> m_smoothNpChannelBoxes;
    QList<QCheckBox *> m_smoothNpCuvetteBoxes;
    QList<QCheckBox *> m_timeDataChannelBoxes;
    QList<QCheckBox *> m_spatialDataChannelBoxes;

    QList<PcrProfile> m_profiles;
    QList<Ct> m_cts;

    void process (void);
    void processCt (void);
    void restoreSettings (void);
    void saveSettings (void);

    bool exportTimeData (const QString &fileName, const QList<TimeData> &data);
    bool exportSpatialData (const QString &fileName, const QList<SpatialData> &data);
    bool exportCt (const QString &fileName);
    bool exportProfile (const QString &fileName);

    static int maskFromUi (const QList<QCheckBox *> &boxes);
    static void maskToUi (const int mask, const QList<QCheckBox *> &boxes);
};

#endif // MAINWINDOW_H
