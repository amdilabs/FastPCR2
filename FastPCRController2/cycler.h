#ifndef CYCLER_H
#define CYCLER_H

#include <QObject>
#include "../../lib/log.h"
#include "../cycler/interface.h"
#include "../cycler/pcrprotocol.h"
#include "../cycler/pge.h"
#include "diskconfiguration.h"
#include "progresstracker.h"

class Cycler : public QObject
{ Q_OBJECT

  public:
    enum State {Closed, Ready, Running};

    static const double pcrHeaterRate;
    static const double heatSealerRate;

    explicit Cycler (Log *log, QObject *parent = nullptr);
    ~Cycler (void);

    bool open (void);
    void close (void);

    bool isOpen (void) const;
    QString version (void) const;

    bool resume (void);
    bool retract (void);

    bool heater65On (const double temperature);
    bool heater95On (const double temperature);
    bool heatSealerOn (const double temperature);
    bool heater65Off (void);
    bool heater95Off (void);
    bool heatSealerOff (void);

    double gripperPosition (const Pge::Select select, const Pge::IoGroup ioGroup);
    double gripperForce (const Pge::Select select, const Pge::IoGroup ioGroup);
    double gripperSpeed (const Pge::Select select, const Pge::IoGroup ioGroup);

    bool start (const PcrProtocol &protocol, const DiskConfiguration &diskConfig, const bool runPreHbh, const bool pcrPause, const bool preHbhPause);
    bool abort (void);

    double rtd65Top1 (void) const;
    double rtd65Top2 (void) const;
    double thermistor65Top (void) const;
    double rtd65Bottom1 (void) const;
    double rtd65Bottom2 (void) const;
    double thermistor65Bottom (void) const;
    double rtd95Top1 (void) const;
    double rtd95Top2 (void) const;
    double thermistor95Top (void) const;
    double rtd95Bottom1 (void) const;
    double rtd95Bottom2 (void) const;
    double thermistor95Bottom (void) const;
    double heatSealerThermistor (void) const;

    bool stepperHome (void);
    bool stepperRetract (void);
    bool stepperMovePositionRelative (const int steps, const double maxSpeed);
    bool stepperMovePositionAbsolute (const int position, const double maxSpeed);
    bool setGripperGroup65 (const Interface::Position position);
    bool setGripperGroup95 (const Interface::Position position);
    bool setGripperGroupHeatSealer (const Interface::Position position);
    bool spindleSpinStart (const double velocity, const double accel, const double decel);
    bool spindleStop (void);
    bool spindleDiskHome (const Interface::Reference reference);
    bool amcUartDisable (void);

  signals:
    void stateChanged (const Cycler::State newState);
    void temperaturesReceived (const QList<double> &top65, const QList<double> &bottom65, const QList<double> &top95, const QList<double> &bottom95, const double heatSealer);
    void started (const int steps);
    void progressed (const int step, const Progress &progress);

  private slots:
    void runDataStreamReceived (const Interface::RunData &runData);
    void tempLogsStreamReceived (const Interface::TempLogs &tempLogs);

  private:
    Log *m_log;
    State m_state;
    Interface m_interface;
    Pge m_pge;
    ProgressTracker m_tracker;
    QString m_version;
    FillingConfiguration m_fillingConfig;
    double m_temperature65;
    double m_temperature95;
    double m_temperatureHeatSealer;

    int m_schedulerIndex;
    bool m_schedulerRunning;

    bool setHeaters (void);
    bool configure (const PcrProtocol &protocol, const DiskConfiguration &diskConfig, const bool runPreHbh, const bool pcrPause, const bool preHbhPause);
    void changeState (const State newState);
};

#endif // CYCLER_H
