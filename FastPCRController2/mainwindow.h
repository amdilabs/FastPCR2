#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCheckBox>
#include <QLineEdit>
#include <QMainWindow>
#include "../../lib/filenamevalidator.h"
#include "../../lib/log.h"
#include "../../lib/signalingelapsedtimer.h"
#include "controller.h"

namespace Ui
{ class MainWindow;
}

class MainWindow : public QMainWindow
{ Q_OBJECT

  public:
    MainWindow (QWidget *parent = nullptr);
    ~MainWindow (void);

  private slots:
    void logReceived (const QDateTime &timestamp, const Log::Severity severity, const QString &source, const QString &text);
    void stateChanged (const Controller::State newState);
    void temperaturesReceived (const QList<double> &top65, const QList<double> &bottom65, const QList<double> &top95, const QList<double> &bottom95, const double heatSealer);
    void started (const int steps);
    void progressed (const int step, const Progress &progress);
    void elapsed (const qint64 time);
    void blinkTimeout (void);

    void actionThermocyclerDiskConfigurationTriggered (void);
    void actionInductionSelectTriggered (void);
    void actionInductionConnectTriggered (void);
    void actionInductionDisconnectTriggered (void);
    void actionOpticsLedConfigurationTriggered (void);
    void actionOpticsCuvetteConfigurationTriggered (void);
    void actionOpticsProcessingConfigurationTriggered (void);
    void actionPcrProtocolOpenTriggered (void);
    void actionPcrProtocolRecentlyUsedTriggered (void);
    void controlTemperatureStandby65BoxEditingFinished (void);
    void controlTemperatureStandby95BoxEditingFinished (void);
    void controlTemperatureStandbyHeatSealerBoxEditingFinished (void);
    void controlStorageDirButtonClicked (void);
    void pcrGraphChannelBoxClicked (void);
    void pcrGraphCuvetteBoxClicked (void);
    void timeDataChannelBoxClicked (void);
    void spatialDataChannelBoxClicked (void);
    void startButtonClicked (void);
    void resumeButtonClicked (void);
    void abortButtonClicked (void);

    void stepperRetractButtonClicked(void);
    void stepperHomeButtonClicked(void);
    void stepperMovePositionRelativeButtonClicked(void);
    void stepperMovePositionAbsoluteButtonClicked(void);
    void sealerClosePushButtonClicked(void);
    void sealerOpenPushButtonClicked(void);
    void heater65ClosePushButtonClicked(void);
    void heater65OpenPushButtonClicked(void);
    void heater95ClosePushButtonClicked(void);
    void heater95OpenPushButtonClicked(void);
    void heaterBothClosePushButtonClicked(void);
    void heaterBothOpenPushButtonClicked(void);
    void spindleSpinStartPushButtonClicked(void);
    void spindleStopPushButtonClicked(void);
    void spindleDiskHomePushButtonClicked(void);
    void spindleMovePositionRelativePushButtonClicked(void);
    void spindleMovePositionAbsolutePushButtonClicked(void);
    void spindleMoveAngleRelativePushButtonClicked(void);
    void spindleMoveAngleAbsolutePushButtonClicked(void);
    void sealerSetpointDoubleSpinBoxEditingFinished();
    void heater65SetpointDoubleSpinBoxEditingFinished();
    void heater95SetpointDoubleSpinBoxEditingFinished();
    void gripperComboBoxActivated(int index);
    void gripperReadButtonClicked(void);
    void gripperWriteButtonClicked(void);
    void amcUartDisablePushButtonClicked(void);
    void led1CurrentDoubleSpinBoxEditingFinished(void);
    void led2CurrentDoubleSpinBoxEditingFinished(void);
    void led3CurrentDoubleSpinBoxEditingFinished(void);
    void led4CurrentDoubleSpinBoxEditingFinished(void);

private:
    Ui::MainWindow *ui;

    Log m_log;
    QString m_portNameInduction;
    Controller m_controller;
    FileNameValidator m_validator;

    Progress::State m_progress;

    SignalingElapsedTimer m_elapsedTimer;
    QTimer m_blinkTimer;
    bool m_blink;

    QStringList m_recentlyUsedProtocols;

    UserInputConfiguration m_userInputConfig;
    QList<QWidget *> m_userInputWidgets;

    QList<QCheckBox *> m_timeDataChannelBoxes;
    QList<QCheckBox *> m_spatialDataChannelBoxes;
    QList<QCheckBox *> m_pcrGraphChannelBoxes;
    QList<QCheckBox *> m_pcrGraphCuvetteBoxes;

    void configureUserInput (void);
    void loadProtocol (const QString &fileName);
    void updateRecentlyUsedProtocols (const QString &fileName);

    void setEnabled (const bool running);
    void resetProgress (void);
    void updateProgress (void);
    void initHeaters (void);
    void updateStatus (void);

    void restoreSettings (void);
    void saveSettings (void);

    void heatersFromJson (const QJsonObject &object);
    QJsonObject heatersToJson (void) const;

    static void updateStatus (QLineEdit *edit, const bool positive, const QString &positiveString, const QString &negativeString);
    static void updateTemperature (QLineEdit *edit, const QVector<double> &temperatures);
    static int maskFromUi (const QList<QCheckBox *> &boxes);
    static void maskToUi (const int mask, const QList<QCheckBox *> &boxes);    
};

#endif // MAINWINDOW_H
