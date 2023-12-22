#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include "../../lib/log.h"
#include "message.h"
#include "slip.h"

class Interface : public QObject
{ Q_OBJECT

  public:
    class AnalogMonitor;
    class AmcEncoder;
    class HeaterGains;
    class RunData;
    class SchedulerStatus;
    class StepperState;
    class Temperature;
    class TempLogs;

    enum Channel {Top65 = 1, Bottom65 = 2, Top95 = 3, Bottom95 = 4, HeatSealer = 5};
    enum Position {Closed = 0, SlightlyOpen = 1, MoreOpen = 2, FullOpen = 3};
    enum Reference {Disk = 0, Index = 1};

    explicit Interface (Log *log, QObject *parent = nullptr);
    ~Interface (void);

    bool isOpen (void) const;

    bool open (const QString &portName);
    void close (void);

    QString getVersion (void);

    bool schedulerStart (void);
    bool schedulerStop (void);
    bool schedulerClear (void);
    SchedulerStatus schedulerGetStatus (void);

    bool schPauseAddPause (void);
    bool schPauseResume (void);

    bool schSpindleDisable (void);
    bool schSpindleStop (void);
    bool schSpindleHome (const Reference reference);
    bool schSpindleMovePositionRelative (const int counts);
    bool schSpindleMovePositionRelativeOpto (const int counts);
    bool schSpindleMoveAngleAbsolute (const int angle, const bool direction);
    bool schSpindleMoveAngleAbsolute_Ming (const int angle, const bool direction);
    bool schSpindleMoveAngleAbsoluteOpto (const int angle, const bool direction);
    bool schSpindleMoveSpeed (const double speed);
    bool schSpindleWaitPosition (void);
    bool schSpindleWaitSpeed (void);
    bool schSpindleSetAccelDecel (const double acceleration, const double deceleration);

    bool schHeaterOn65c (const double temperature, const double rate);
    bool schHeaterOn95c (const double temperature, const double rate);
    bool schHeaterOnSealer (const double temperature, const double rate);
    bool schHeaterOff65c (void);
    bool schHeaterOff95c (void);
    bool schHeaterOffSealer (void);
    bool schHeaterOffAll (void);
    bool schHeaterWaitTemperature65c (void);
    bool schHeaterWaitTemperature95c (void);
    bool schHeaterWaitTemperatureSealer (void);

    bool schGripper65cToPos (const Position position);
    bool schGripper95cToPos (const Position position);
    bool schGripperSealerToPos (const Position position);
    bool schGripperAllToPos (const Position position);
    bool schGripperWaitPosition65c (void);
    bool schGripperWaitForce65c (void);
    bool schGripperWaitPosition95c (void);
    bool schGripperWaitForce95c (void);
    bool schGripperWaitPositionSealer (void);
    bool schGripperWaitForceSealer (void);

    bool schStepperHome (const bool direction);
    bool schStepperMovePositionRelative (const int steps, const double maxSpeed);
    bool schStepperMovePositionAbsolute (const int position, const double maxSpeed);
    bool schStepperWaitPosition (void);
    bool schStepperWaitHome (void);

    bool schTimerStart0 (void);
    bool schTimerStart1 (void);
    bool schTimerStart2 (void);
    bool schTimerWait0 (const int timeout);
    bool schTimerWait1 (const int timeout);
    bool schTimerWait2 (const int timeout);

    bool schLoopSetMarker (void);
    bool schLoopGotoMarker (const int numOfLoops);

    bool schDoorOpen (void);

    bool amcWrite (const int index, const int offset, const QByteArray &data);
    QByteArray amcRead (const int index, const int offset, const int size);
    bool amcSetInput (const int index, const bool value);
    bool amcGetInput (const int index);
    int amcGetVelocity (void);
    int amcGetPosition (void);
    bool amcMoveAtVelocity (const double velocity);
    bool amcEnablePositionMode (void);
    bool amcMoveRelative (const int counts);
    bool amcMoveAbsolute (const int counts, const bool direction);
    bool amcUartDisable (void);
    bool amcHomeToIndex (void);
    bool amcHomeToFlag (void);
    AmcEncoder amcGetEncoder (void);

    StepperState getStepperState (void);
    bool setStepperEnable (const bool enable);
    bool stepperHome (const int microsteps, const int speed, const int direction);
    bool stepperMoveAbsolute (const int microsteps, const int speed);
    bool stepperMoveRelative (const int microsteps, const int speed);
    int stepperGetPosition (void);

    bool disableAllHeaters (void);
    bool setEnable65Heaters (const double temperature, const double rate);
    bool disable65Heaters (void);
    bool setEnable95Heaters (const double temperature, const double rate);
    bool disable95Heaters (void);
    bool setEnableHeatSealer (const double temperature, const double rate);
    bool disableHeatSealer (void);

    bool setGripperGroup65 (const Position position);
    bool setGripperGroup95 (const Position position);
    bool setGripperGroupHeatSealer (const Position position);
    bool setDoorOpen (void);
    int sendGripper485 (const int select, const int address, const int function, const int regAddress, const int data);

    bool set65HeaterGains (const double kpOuter, const double kiOuter, const double kpInner, const double kiInner, const double rateGain = 0.0);
    HeaterGains get65HeaterGains (void);
    bool set95HeaterGains (const double kpOuter, const double kiOuter, const double kpInner, const double kiInner, const double rateGain = 0.0);
    HeaterGains get95HeaterGains (void);
    bool setHeatSealerGains (const double kpOuter, const double kiOuter, const double kpInner, const double kiInner, const double rateGain = 0.0);
    HeaterGains getHeatSealerGains (void);

    bool setTempControlStream (const Channel channel, const int divisor);
    bool setRtdTempStream (const int divisor);
    bool setThermistorTempStream (const int divisor);
    bool setRunDataStream (const int divisor);
    bool setSchedulerStatusStream (const int divisor);
    bool setTempLogsStream (const int divisor);

    QList<Temperature> getRtdTemps (void);
    QList<Temperature> getThermistorTemps (void);
    AnalogMonitor getAnalogMonitor (void);

    static QList<QString> listDevices (void);

  signals:
    void tempControlStreamReceived (void);
    void rtdTempStreamReceived (const QList<Interface::Temperature> &temperatures);
    void thermistorTempStreamReceived (const QList<Interface::Temperature> &temperatures);
    void runDataStreamReceived (const Interface::RunData &runData);
    void schedulerStatusStreamReceived (const Interface::SchedulerStatus &schedulerStatus);
    void tempLogsStreamReceived (const Interface::TempLogs &tempLogs);

  private slots:
    void packetReceived (const QByteArray &packet);

  private:
    static const int vendorId = 0x0483;
    static const int productId = 0x5740;

    static const int GET_VERSION = 0x00;

    static const int SCHEDULER_START = 0x05;
    static const int SCHEDULER_STOP = 0x06;
    static const int SCHEDULER_CLEAR = 0x07;
    static const int SCHEDULER_GET_STATUS = 0x08;

    static const int SCH_PAUSE_ADD_PAUSE = 0x09;
    static const int SCH_PAUSE_RESUME = 0x0a;

    static const int SCH_SPINDLE_MOVE_ANGLE_ABSOLUTE_Ming = 0x14;
    static const int SCH_SPINDLE_DISABLE = 0x15;
    static const int SCH_SPINDLE_STOP = 0x16;
    static const int SCH_SPINDLE_HOME = 0x17;
    static const int SCH_SPINDLE_MOVE_POSITION_RELATIVE = 0x18;
    static const int SCH_SPINDLE_MOVE_POSITION_RELATIVE_OPTO = 0x19;
    static const int SCH_SPINDLE_MOVE_ANGLE_ABSOLUTE = 0x1a;
    static const int SCH_SPINDLE_MOVE_ANGLE_ABSOLUTE_OPTO = 0x1b;
    static const int SCH_SPINDLE_MOVE_SPEED = 0x1c;
    static const int SCH_SPINDLE_WAIT_POSITION = 0x1d;
    static const int SCH_SPINDLE_WAIT_SPEED = 0x1e;
    static const int SCH_SPINDLE_SET_ACCEL_DECEL = 0x1f;

    static const int SCH_HEATER_ON_65C = 0x20;
    static const int SCH_HEATER_ON_95C = 0x21;
    static const int SCH_HEATER_ON_SEALER = 0x22;
    static const int SCH_HEATER_OFF_65C = 0x23;
    static const int SCH_HEATER_OFF_95C = 0x24;
    static const int SCH_HEATER_OFF_SEALER = 0x25;
    static const int SCH_HEATER_OFF_ALL = 0x26;
    static const int SCH_HEATER_WAIT_TEMPERATURE_65C = 0x27;
    static const int SCH_HEATER_WAIT_TEMPERATURE_95C = 0x28;
    static const int SCH_HEATER_WAIT_TEMPERATURE_SEALER = 0x29;

    static const int SCH_GRIPPER_65C_TO_POS = 0x30;
    static const int SCH_GRIPPER_95C_TO_POS = 0x31;
    static const int SCH_GRIPPER_SEALER_TO_POS = 0x32;
    static const int SCH_GRIPPER_ALL_TO_POS = 0x33;
    static const int SCH_GRIPPER_WAIT_POSITION_65C = 0x34;
    static const int SCH_GRIPPER_WAIT_FORCE_65C = 0x35;
    static const int SCH_GRIPPER_WAIT_POSITION_95C = 0x36;
    static const int SCH_GRIPPER_WAIT_FORCE_95C = 0x37;
    static const int SCH_GRIPPER_WAIT_POSITION_SEALER = 0x38;
    static const int SCH_GRIPPER_WAIT_FORCE_SEALER = 0x39;

    static const int SCH_STEPPER_HOME = 0x40;
    static const int SCH_STEPPER_MOVE_POSITION_RELATIVE = 0x41;
    static const int SCH_STEPPER_MOVE_POSITION_ABSOLUTE = 0x42;
    static const int SCH_STEPPER_WAIT_POSITION = 0x43;
    static const int SCH_STEPPER_WAIT_HOME = 0x44;

    static const int SCH_TIMER_START_0 = 0x45;
    static const int SCH_TIMER_START_1 = 0x46;
    static const int SCH_TIMER_START_2 = 0x47;
    static const int SCH_TIMER_WAIT_0 = 0x48;
    static const int SCH_TIMER_WAIT_1 = 0x49;
    static const int SCH_TIMER_WAIT_2 = 0x4a;

    static const int SCH_LOOP_SET_MARKER = 0x4b;
    static const int SCH_LOOP_GOTO_MARKER = 0x4c;

    static const int SCH_DOOR_OPEN = 0x4d;

    static const int AMC_WRITE = 0x50;
    static const int AMC_READ = 0x51;
    static const int AMC_SET_INPUT = 0x52;
    static const int AMC_GET_INPUT = 0x53;
    static const int AMC_GET_VELOCITY = 0x54;
    static const int AMC_GET_POSITION = 0x55;
    static const int AMC_MOVE_AT_VELOCITY = 0x56;
    static const int AMC_ENABLE_POSITION_MODE = 0x57;
    static const int AMC_MOVE_RELATIVE = 0x58;
    static const int AMC_MOVE_ABSOLUTE = 0x59;
    static const int AMC_UART_DISABLE = 0x5a;
    static const int AMC_HOME_TO_INDEX = 0x5b;
    static const int AMC_HOME_TO_FLAG = 0x5c;
    static const int AMC_GET_ENCODER = 0x5d;
    static const int AMC_MOVE_RELATIVE_OPTO = 0x5e;

    static const int GET_STEPPER_STATE = 0x60;
    static const int SET_STEPPER_ENABLE = 0x61;
    static const int STEPPER_HOME = 0x62;
    static const int STEPPER_MOVE_ABSOLUTE = 0x63;
    static const int STEPPER_MOVE_RELATIVE = 0x64;
    static const int STEPPER_GET_POSITION = 0x65;

    static const int DISABLE_ALL_HEATERS = 0x70;
    static const int SET_ENABLE_65_HEATERS = 0x71;
    static const int DISABLE_65_HEATERS = 0x72;
    static const int SET_ENABLE_95_HEATERS = 0x73;
    static const int DISABLE_95_HEATERS = 0x74;
    static const int SET_ENABLE_HEAT_SEALER = 0x75;
    static const int DISABLE_HEAT_SEALER = 0x76;
    static const int SET_GRIPPER_GROUP_65 = 0x77;
    static const int SET_GRIPPER_GROUP_95 = 0x78;
    static const int SET_GRIPPER_GROUP_HEAT_SEALER = 0x79;
    static const int SET_DOOR_OPEN = 0x7a;
    static const int SEND_GRIPPER_485 = 0x7c;

    static const int SET_65_HEATER_GAINS = 0x80;
    static const int GET_65_HEATER_GAINS = 0x81;
    static const int SET_95_HEATER_GAINS = 0x82;
    static const int GET_95_HEATER_GAINS = 0x83;
    static const int SET_HEAT_SEALER_GAINS = 0x84;
    static const int GET_HEAT_SEALER_GAINS = 0x85;

    static const int SET_TEMP_CONTROL_STREAM = 0x90;
    static const int SET_RTD_TEMP_STREAM = 0x91;
    static const int SET_THERMISTOR_TEMP_STREAM = 0x92;
    static const int SET_RUN_DATA_STREAM = 0x93;
    static const int SET_SCHEDULER_STATUS_STREAM = 0x94;
    static const int SET_TEMP_LOGS_STREAM = 0x95;

    static const int GET_RTD_TEMPS = 0x9a;
    static const int GET_THERMISTOR_TEMPS = 0x9b;
    static const int GET_ANALOG_MONITOR = 0x9c;

    static const int STREAM_TEMP_CONTROL = 0xa0;
    static const int STREAM_RTD_TEMP_SENSORS = 0xa1;
    static const int STREAM_THERMISTOR_TEMP_SENSORS = 0xa2;
    static const int STREAM_RUN_DATA = 0xa3;
    static const int STREAM_SCHEDULER_STATUS = 0xa4;
    static const int STREAM_TEMP_LOGS = 0xa5;

    static const int COMMAND_ERROR = 0xf0;
    static const int CRC_ERROR = 0xf1;

    Log *m_log;
    Slip m_slip;
    int m_id;

    int m_responseType;
    int m_responseId;
    bool m_responseReceived;
    Message m_response;

    Message write (const Message &message);
    Message createMessage (const int type);
};

class Interface::AnalogMonitor
{ public:
    AnalogMonitor (const Message &message);

    double vMotor (void) const;
    double v24Sw (void) const;
    double v24 (void) const;
    double v5 (void) const;
    double v3p3 (void) const;
    double laserSense (void) const;
    double spindleOpto (void) const;
    double auxLimit1 (void) const;
    double auxLimit2 (void) const;

  private:
    double m_vMotor;
    double m_v24Sw;
    double m_v24;
    double m_v5;
    double m_v3p3;
    double m_laserSense;
    double m_spindleOpto;
    double m_auxLimit1;
    double m_auxLimit2;
};

class Interface::AmcEncoder
{ public:
    AmcEncoder (const Message &message);

    int encoderCounts (void) const;
    int indexCapture (void) const;
    int out1 (void) const;
    int out2 (void) const;

  private:
    int m_encoderCounts;
    int m_indexCapture;
    int m_out1;
    int m_out2;
};

class Interface::HeaterGains
{ public:
    HeaterGains (const Message &message);

    double kpOuterTop (void) const;
    double kiOuterTop (void) const;
    double kpInnerTop (void) const;
    double kiInnerTop (void) const;
    double kpOuterBottom (void) const;
    double kiOuterBottom (void) const;
    double kpInnerBottom (void) const;
    double kiInnerBottom (void) const;

  private:
    double m_kpOuterTop;
    double m_kiOuterTop;
    double m_kpInnerTop;
    double m_kiInnerTop;
    double m_kpOuterBottom;
    double m_kiOuterBottom;
    double m_kpInnerBottom;
    double m_kiInnerBottom;
};

class Interface::RunData
{ public:
    RunData (const Message &message);

    bool schedulerRunning (void) const;
    int schedulerIndex (void) const;
    int spindleAngle (void) const;
    int gantryPosition (void) const;
    double temperature65Top (void) const;
    double temperature65Bottom (void) const;
    double temperature95Top (void) const;
    double temperature95Bottom (void) const;
    double temperatureHeatSealer (void) const;

  private:
    bool m_schedulerRunning;
    int m_schedulerIndex;
    int m_spindleAngle;
    int m_gantryPosition;
    double m_temperature65Top;
    double m_temperature65Bottom;
    double m_temperature95Top;
    double m_temperature95Bottom;
    double m_temperatureHeatSealer;
};

class Interface::SchedulerStatus
{ public:
    SchedulerStatus (const Message &message);

    bool runningFlag (void) const;
    bool errorFlag (void) const;
    int errorStep (void) const;
    int currentStep (void) const;
    int totalSteps (void) const;
    int remainingSpindleBuffer (void) const;
    int remainingHeaterBuffer (void) const;
    int remainingGripperBuffer (void) const;
    int remainingStepperBuffer (void) const;
    int remainingDoorBuffer (void) const;
    int remainingTimerBuffer (void) const;
    int remainingLoopBuffer (void) const;

  private:
    bool m_runningFlag;
    bool m_errorFlag;
    int m_errorStep;
    int m_currentStep;
    int m_totalSteps;
    int m_remainingSpindleBuffer;
    int m_remainingHeaterBuffer;
    int m_remainingGripperBuffer;
    int m_remainingStepperBuffer;
    int m_remainingDoorBuffer;
    int m_remainingTimerBuffer;
    int m_remainingLoopBuffer;
};

class Interface::StepperState
{ public:
    StepperState (const Message &message);

    int mode (void) const;
    int homeState (void) const;

  private:
    int m_mode;
    int m_homeState;
};

class Interface::Temperature
{ public:
    static QList<Temperature> temperatures (const Message &message);

    bool isValid (void) const;
    double temperature (void) const;
    double rate (void) const;
    int lastRead (void) const;

  private:
    bool m_isValid;
    double m_temperature;
    double m_rate;
    int m_lastRead;

    Temperature (const Message &message, const int offset);
};

class Interface::TempLogs
{ public:
    TempLogs (const Message &message);

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

  private:
    double m_rtd65Top1;
    double m_rtd65Top2;
    double m_thermistor65Top;
    double m_rtd65Bottom1;
    double m_rtd65Bottom2;
    double m_thermistor65Bottom;
    double m_rtd95Top1;
    double m_rtd95Top2;
    double m_thermistor95Top;
    double m_rtd95Bottom1;
    double m_rtd95Bottom2;
    double m_thermistor95Bottom;
    double m_heatSealerThermistor;
};

#endif // INTERFACE_H
