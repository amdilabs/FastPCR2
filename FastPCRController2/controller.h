#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QTimer>
#include "../../lib/log.h"
#include "../../lib/ui/userinput/userinputdata.h"
#include "../filewriter.h"
#include "../induction/induction.h"
#include "../optics/cuvettelocationconfigurationcontainer.h"
#include "../optics/cuvetteprocessingconfiguration.h"
#include "../optics/ledconfigurationcontainer.h"
#include "../optics/opticsdevice.h"
#include "../optics/pcrprofile.h"
#include "../optics/timedata.h"
#include "../optics/spatialdata.h"
#include "cycler.h"
#include "diskconfiguration.h"

class Controller : public QObject
{ Q_OBJECT

  public:
    enum State {NotReady, Ready, Running, PcrPaused, PreHbhPaused};

    explicit Controller (Log *log, QObject *parent = nullptr);
    ~Controller (void);

    bool isOpenCycler (void) const;
    bool isOpenInduction (void) const;
    bool isOpenOptics (void) const;

    bool openCycler (void);
    bool openInduction (const QString &portName);
    bool openOptics (void);

    void closeCycler (void);
    void closeInduction (void);
    void closeOptics (void);

    bool needInduction (void) const;
    bool hasProtocol (void) const;
    PcrProtocol protocol (void) const;
    bool setProtocol (const PcrProtocol &protocol);

    DiskConfiguration diskConfig (void) const;
    void setDiskConfig (const DiskConfiguration &config);

    LedConfigurationContainer ledConfig (void) const;
    int ledEnable (void) const;
    bool setLedConfig (const LedConfigurationContainer &config);
    bool setLedEnable (const int ledMask);
    void ledOnEngineering (const int channel, double current, double shunt);
    void ledOffEngineering (const int channel);

    int sealerEnable (void) const;
    bool setSealerEnable (const int sealerMask);

    CuvetteLocationConfigurationContainer cuvetteConfig (void) const;
    void setCuvetteConfig (const CuvetteLocationConfigurationContainer &config);

    CuvetteProcessingConfiguration processingConfig (void) const;
    void setProcessingConfig (const CuvetteProcessingConfiguration &config);

    bool start (const QString &directory, const QString &fileNameSuffix, const bool runPreHbh, const bool pcrPause, const bool preHbhPause, const UserInputData &userInput);
    bool resume (void);
    void abort (void);

    bool standby65On (const double temperature);
    bool standby95On (const double temperature);
    bool standbySealerOn (const double temperature);
    bool standby65Off (void);
    bool standby95Off (void);
    bool standbySealerOff (void);

    bool stepperHome (void);
    bool stepperRetract (void);
    bool stepperMovePositionRelative (const int steps, const double maxSpeed);
    bool stepperMovePositionAbsolute (const int position, const double maxSpeed);
    bool setSealerClose(void);
    bool setSealerOpen(void);
    bool setHeater65Close(void);
    bool setHeater65Open(void);
    bool setHeater95Close(void);
    bool setHeater95Open(void);
    bool setHeaterBothClose(void);
    bool setHeaterBothOpen(void);
    bool spindleSpinStart(const double velocity, const double accel, const double decel);
    bool spindleStop(void);
    bool spindleDiskHome(const Interface::Reference reference);
    bool setSealerOn(const double temperature);
    bool setSealerOff(void);
    bool setHeater65On(const double temperature);
    bool setHeater65Off(void);
    bool setHeater95On(const double temperature);
    bool setHeater95Off(void);
    bool amcUartDisable (void);

  signals:
    void stateChanged (const Controller::State newState);
    void temperaturesReceived (const QVector<double> &top65, const QVector<double> &bottom65, const QVector<double> &top95, const QVector<double> &bottom95, const double heatSealer);
    void started (const int steps);
    void progressed (const int step, const Progress &progress);
    void timeDataProcessed (const QList<TimeData> &data);
    void spatialDataProcessed (const QList<SpatialData> &data);
    void cycleCompleted (const QList<PcrProfile> &profiles);

  private slots:
    void logReceived (const QDateTime &timestamp, const Log::Severity severity, const QString &source, const QString &text);
    void cyclerStateChanged (const Cycler::State newState);
    void cyclerTemperaturesReceived (const QVector<double> &top65, const QVector<double> &bottom65, const QVector<double> &top95, const QVector<double> &bottom95, const double heatSealer);
    void cyclerStarted (const int steps);
    void cyclerProgressed (const int step, const Progress &progress);
    void opticsDataReceived (const RawData &data, const CaptureConfiguration &config);
    void opticsEncoderReceived (const QByteArray &data);
    void inductionTimeout (void);
    void laserTimeout (void);

  private:
    static const int laserChannel;

    State m_state;

    Log *m_log;
    Cycler m_cycler;
    Induction m_induction;
    OpticsDevice m_optics;

    bool m_hasProtocol;
    PcrProtocol m_protocol;

    DiskConfiguration m_diskConfig;

    LedConfigurationContainer m_ledConfig;
    int m_ledMask;
    CaptureConfiguration m_captureConfig;
    CuvetteLocationConfigurationContainer m_cuvetteConfig;
    CuvetteProcessingConfiguration m_processingConfig;
    QMap<CuvetteId, PcrProfile> m_profiles;

    FileWriter m_writer;
    QElapsedTimer m_elapsedTimer;
    QTimer m_inductionTimer;
    QTimer m_laserTimer;

    bool m_isThermocycling;
    bool m_inductionActive;
    bool m_laserActive;
    bool m_diskIndexEnabled;

    void checkReady (void);
    void stepperCheckReady (void);
    void changeState (const State newState);

    bool updateLeds (void);
    bool configureLeds (void);

    bool openWriter (const QString &directory, const QString &fileNameSuffix, const UserInputData &userInput);
    bool writeGripperConfig (void);    
};

#endif // CONTROLLER_H
