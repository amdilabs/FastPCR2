#include <QCoreApplication>
#include <QElapsedTimer>
#include <QSerialPortInfo>
#include <QThread>
#include "interface.h"

Interface::Interface (Log *log, QObject *parent) : QObject (parent), m_log (log), m_slip (log)
{ connect (&m_slip, &Slip::packetReceived, this, &Interface::packetReceived);
}

Interface::~Interface (void)
{ close ();
}

bool Interface::isOpen (void) const
{ return m_slip.isOpen ();
}

bool Interface::open (const QString &portName)
{ if (!m_slip.open (portName)) return false;

  m_id = 0;
  return true;
}

void Interface::close (void)
{ m_slip.close ();
}

QString Interface::getVersion (void)
{ return write (createMessage (GET_VERSION)).getString (0);
}

bool Interface::schedulerStart (void)
{ return write (createMessage (SCHEDULER_START)).isValid ();
}

bool Interface::schedulerStop (void)
{ return write (createMessage (SCHEDULER_STOP)).isValid ();
}

bool Interface::schedulerClear (void)
{ return write (createMessage (SCHEDULER_CLEAR)).isValid ();
}

Interface::SchedulerStatus Interface::schedulerGetStatus (void)
{ return SchedulerStatus (write (createMessage (SCHEDULER_GET_STATUS)));
}

bool Interface::schPauseAddPause (void)
{ return write (createMessage (SCH_PAUSE_ADD_PAUSE)).isValid ();
}

bool Interface::schPauseResume (void)
{ return write (createMessage (SCH_PAUSE_RESUME)).isValid ();
}

bool Interface::schSpindleDisable (void)
{ return write (createMessage (SCH_SPINDLE_DISABLE)).isValid ();
}

bool Interface::schSpindleStop (void)
{ return write (createMessage (SCH_SPINDLE_STOP)).isValid ();
}

bool Interface::schSpindleHome (const Reference reference)
{ Message message = createMessage (SCH_SPINDLE_HOME);
  message.appendInt8 (reference);
  return write (message).isValid ();
}

bool Interface::schSpindleMovePositionRelative (const int counts)
{ Message message = createMessage (SCH_SPINDLE_MOVE_POSITION_RELATIVE);
  message.appendInt32 (counts);
  return write (message).isValid ();
}

bool Interface::schSpindleMovePositionRelativeOpto (const int counts)
{ Message message = createMessage (SCH_SPINDLE_MOVE_POSITION_RELATIVE_OPTO);
  message.appendInt32 (counts);
  return write (message).isValid ();
}

bool Interface::schSpindleMoveAngleAbsolute (const int angle, const bool direction)
{ Message message = createMessage (SCH_SPINDLE_MOVE_ANGLE_ABSOLUTE);
  message.appendInt32 (angle);
  message.appendInt32 (direction?1:0);
  return write (message).isValid ();
}

bool Interface::schSpindleMoveAngleAbsolute_Ming (const int angle, const bool direction)
{ Message message = createMessage (SCH_SPINDLE_MOVE_ANGLE_ABSOLUTE_Ming);
  message.appendInt32 (angle);
  message.appendInt32 (direction?1:0);
  return write (message).isValid ();
}

bool Interface::schSpindleMoveAngleAbsoluteOpto (const int angle, const bool direction)
{ Message message = createMessage (SCH_SPINDLE_MOVE_ANGLE_ABSOLUTE_OPTO);
  message.appendInt32 (angle);
  message.appendInt32 (direction?1:0);
  return write (message).isValid ();
}

bool Interface::schSpindleMoveSpeed (const double speed)
{ Message message = createMessage (SCH_SPINDLE_MOVE_SPEED);
  message.appendFloat (speed);
  return write (message).isValid ();
}

bool Interface::schSpindleWaitPosition (void)
{ return write (createMessage (SCH_SPINDLE_WAIT_POSITION)).isValid ();
}

bool Interface::schSpindleWaitSpeed (void)
{ return write (createMessage (SCH_SPINDLE_WAIT_SPEED)).isValid ();
}

bool Interface::schSpindleSetAccelDecel (const double acceleration, const double deceleration)
{ Message message = createMessage (SCH_SPINDLE_SET_ACCEL_DECEL);
  message.appendFloat (acceleration);
  message.appendFloat (deceleration);
  return write (message).isValid ();
}

bool Interface::schHeaterOn65c (const double temperature, const double rate)
{ Message message = createMessage (SCH_HEATER_ON_65C);
  message.appendFloat (temperature);
  message.appendFloat (rate);
  return write (message).isValid ();
}

bool Interface::schHeaterOn95c (const double temperature, const double rate)
{ Message message = createMessage (SCH_HEATER_ON_95C);
  message.appendFloat (temperature);
  message.appendFloat (rate);
  return write (message).isValid ();
}

bool Interface::schHeaterOnSealer (const double temperature, const double rate)
{ Message message = createMessage (SCH_HEATER_ON_SEALER);
  message.appendFloat (temperature);
  message.appendFloat (rate);
  return write (message).isValid ();
}

bool Interface::schHeaterOff65c (void)
{ return write (createMessage (SCH_HEATER_OFF_65C)).isValid ();
}

bool Interface::schHeaterOff95c (void)
{ return write (createMessage (SCH_HEATER_OFF_95C)).isValid ();
}

bool Interface::schHeaterOffSealer (void)
{ return write (createMessage (SCH_HEATER_OFF_SEALER)).isValid ();
}

bool Interface::schHeaterOffAll (void)
{ return write (createMessage (SCH_HEATER_OFF_ALL)).isValid ();
}

bool Interface::schHeaterWaitTemperature65c (void)
{ return write (createMessage (SCH_HEATER_WAIT_TEMPERATURE_65C)).isValid ();
}

bool Interface::schHeaterWaitTemperature95c (void)
{ return write (createMessage (SCH_HEATER_WAIT_TEMPERATURE_95C)).isValid ();
}

bool Interface::schHeaterWaitTemperatureSealer (void)
{ return write (createMessage (SCH_HEATER_WAIT_TEMPERATURE_SEALER)).isValid ();
}

bool Interface::schGripper65cToPos (const Position position)
{ Message message = createMessage (SCH_GRIPPER_65C_TO_POS);
  message.appendInt8 (position);
  return write (message).isValid ();
}

bool Interface::schGripper95cToPos (const Position position)
{ Message message = createMessage (SCH_GRIPPER_95C_TO_POS);
  message.appendInt8 (position);
  return write (message).isValid ();
}

bool Interface::schGripperSealerToPos (const Position position)
{ Message message = createMessage (SCH_GRIPPER_SEALER_TO_POS);
  message.appendInt8 (position);
  return write (message).isValid ();
}

bool Interface::schGripperAllToPos (const Position position)
{ Message message = createMessage (SCH_GRIPPER_ALL_TO_POS);
  message.appendInt8 (position);
  return write (message).isValid ();
}

bool Interface::schGripperWaitPosition65c (void)
{ return write (createMessage (SCH_GRIPPER_WAIT_POSITION_65C)).isValid ();
}

bool Interface::schGripperWaitForce65c (void)
{ return write (createMessage (SCH_GRIPPER_WAIT_FORCE_65C)).isValid ();
}

bool Interface::schGripperWaitPosition95c (void)
{ return write (createMessage (SCH_GRIPPER_WAIT_POSITION_95C)).isValid ();
}

bool Interface::schGripperWaitForce95c (void)
{ return write (createMessage (SCH_GRIPPER_WAIT_FORCE_95C)).isValid ();
}

bool Interface::schGripperWaitPositionSealer (void)
{ return write (createMessage (SCH_GRIPPER_WAIT_POSITION_SEALER)).isValid ();
}

bool Interface::schGripperWaitForceSealer (void)
{ return write (createMessage (SCH_GRIPPER_WAIT_FORCE_SEALER)).isValid ();
}

bool Interface::schStepperHome (const bool direction)
{ Message message = createMessage (SCH_STEPPER_HOME);
  message.appendBool (direction);
  return write (message).isValid ();
}

bool Interface::schStepperMovePositionRelative (const int steps, const double maxSpeed)
{ Message message = createMessage (SCH_STEPPER_MOVE_POSITION_RELATIVE);
  message.appendInt32 (steps);
  message.appendFloat (maxSpeed);
  return write (message).isValid ();
}

bool Interface::schStepperMovePositionAbsolute (const int position, const double maxSpeed)
{ Message message = createMessage (SCH_STEPPER_MOVE_POSITION_ABSOLUTE);
  message.appendInt32 (position);
  message.appendFloat (maxSpeed);
  return write (message).isValid ();
}

bool Interface::schStepperWaitPosition (void)
{ return write (createMessage (SCH_STEPPER_WAIT_POSITION)).isValid ();
}

bool Interface::schStepperWaitHome (void)
{ return write (createMessage (SCH_STEPPER_WAIT_HOME)).isValid ();
}

bool Interface::schTimerStart0 (void)
{ return write (createMessage (SCH_TIMER_START_0)).isValid ();
}

bool Interface::schTimerStart1 (void)
{ return write (createMessage (SCH_TIMER_START_1)).isValid ();
}

bool Interface::schTimerStart2 (void)
{ return write (createMessage (SCH_TIMER_START_2)).isValid ();
}

bool Interface::schTimerWait0 (const int timeout)
{ Message message = createMessage (SCH_TIMER_WAIT_0);
  message.appendInt32 (timeout);
  return write (message).isValid ();
}

bool Interface::schTimerWait1 (const int timeout)
{ Message message = createMessage (SCH_TIMER_WAIT_1);
  message.appendInt32 (timeout);
  return write (message).isValid ();
}

bool Interface::schTimerWait2 (const int timeout)
{ Message message = createMessage (SCH_TIMER_WAIT_2);
  message.appendInt32 (timeout);
  return write (message).isValid ();
}

bool Interface::schLoopSetMarker (void)
{ return write (createMessage (SCH_LOOP_SET_MARKER)).isValid ();
}

bool Interface::schLoopGotoMarker (const int numOfLoops)
{ Message message = createMessage (SCH_LOOP_GOTO_MARKER);
  message.appendInt32 (numOfLoops);
  return write (message).isValid ();
}

bool Interface::schDoorOpen (void)
{ return write (createMessage (SCH_DOOR_OPEN)).isValid ();
}

bool Interface::amcWrite (const int index, const int offset, const QByteArray &data)
{ Message message = createMessage (AMC_WRITE);
  message.appendInt8 (index);
  message.appendInt8 (offset);
  message.appendArray (data);
  return write (message).isValid ();
}

QByteArray Interface::amcRead (const int index, const int offset, const int size)
{ Message message = createMessage (AMC_READ);
  message.appendInt8 (index);
  message.appendInt8 (offset);
  message.appendInt8 (size);
  return write (message).getArray (0);
}

bool Interface::amcSetInput (const int index, const bool value)
{ Message message = createMessage (AMC_SET_INPUT);
  message.appendInt8 (index);
  message.appendBool (value);
  return write (message).isValid ();
}

bool Interface::amcGetInput (const int index)
{ Message message = createMessage (AMC_GET_INPUT);
  message.appendInt8 (index);

  Message response = write (message);
  return (response.isValid ()?response.getBool (0):false);
}

int Interface::amcGetVelocity (void)
{ Message response = write (createMessage (AMC_GET_VELOCITY));
  return (response.isValid ()?response.getInt32 (0):0);
}

int Interface::amcGetPosition (void)
{ Message response = write (createMessage (AMC_GET_POSITION));
  return (response.isValid ()?response.getInt32 (0):0);
}

bool Interface::amcMoveAtVelocity (const double velocity)
{ Message message = createMessage (AMC_MOVE_AT_VELOCITY);
  message.appendFloat (velocity);
  return write (message).isValid ();
}

bool Interface::amcEnablePositionMode (void)
{ return write (createMessage (AMC_ENABLE_POSITION_MODE)).isValid ();
}

bool Interface::amcMoveRelative (const int counts)
{ Message message = createMessage (AMC_MOVE_RELATIVE);
  message.appendInt32 (counts);
  return write (message).isValid ();
}

bool Interface::amcMoveAbsolute (const int counts, const bool direction)
{ Message message = createMessage (AMC_MOVE_ABSOLUTE);
  message.appendInt32 (counts);
  message.appendInt32 (direction?1:0);
  return write (message).isValid ();
}

bool Interface::amcUartDisable (void)
{ printf("AMC UART Disable Sent");
  return write (createMessage (AMC_UART_DISABLE)).isValid ();
}

bool Interface::amcHomeToIndex (void)
{ return write (createMessage (AMC_HOME_TO_INDEX)).isValid ();
}

bool Interface::amcHomeToFlag (void)
{ return write (createMessage (AMC_HOME_TO_FLAG)).isValid ();
}

Interface::AmcEncoder Interface::amcGetEncoder (void)
{ return AmcEncoder (write (createMessage (AMC_GET_ENCODER)));
}

Interface::StepperState Interface::getStepperState (void)
{ return StepperState (write (createMessage (GET_STEPPER_STATE)));
}

bool Interface::setStepperEnable (const bool enable)
{ Message message = createMessage (SET_STEPPER_ENABLE);
  message.appendBool (enable);
  return write (message).isValid ();
}

bool Interface::stepperHome (const int microsteps, const int speed, const int direction)
{ Message message = createMessage (STEPPER_HOME);
  message.appendInt32 (microsteps);
  message.appendInt32 (speed);
  message.appendInt32 (direction);
  return write (message).isValid ();
}

bool Interface::stepperMoveAbsolute (const int microsteps, const int speed)
{ Message message = createMessage (STEPPER_MOVE_ABSOLUTE);
  message.appendInt32 (microsteps);
  message.appendInt32 (speed);
  return write (message).isValid ();
}

bool Interface::stepperMoveRelative (const int microsteps, const int speed)
{ Message message = createMessage (STEPPER_MOVE_RELATIVE);
  message.appendInt32 (microsteps);
  message.appendInt32 (speed);
  return write (message).isValid ();
}

int Interface::stepperGetPosition (void)
{ Message response = write (createMessage (STEPPER_GET_POSITION));
  return (response.isValid ()?response.getInt32 (0):0);
}

bool Interface::disableAllHeaters (void)
{ return write (createMessage (DISABLE_ALL_HEATERS)).isValid ();
}

bool Interface::setEnable65Heaters (const double temperature, const double rate)
{ Message message = createMessage (SET_ENABLE_65_HEATERS);
  message.appendFloat (temperature);
  message.appendFloat (rate);
  return write (message).isValid ();
}

bool Interface::disable65Heaters (void)
{ return write (createMessage (DISABLE_65_HEATERS)).isValid ();
}

bool Interface::setEnable95Heaters (const double temperature, const double rate)
{ Message message = createMessage (SET_ENABLE_95_HEATERS);
  message.appendFloat (temperature);
  message.appendFloat (rate);
  return write (message).isValid ();
}

bool Interface::disable95Heaters (void)
{ return write (createMessage (DISABLE_95_HEATERS)).isValid ();
}

bool Interface::setEnableHeatSealer (const double temperature, const double rate)
{ Message message = createMessage (SET_ENABLE_HEAT_SEALER);
  message.appendFloat (temperature);
  message.appendFloat (rate);
  return write (message).isValid ();
}

bool Interface::disableHeatSealer (void)
{ return write (createMessage (DISABLE_HEAT_SEALER)).isValid ();
}

bool Interface::setGripperGroup65 (const Position position)
{ Message message = createMessage (SET_GRIPPER_GROUP_65);
  message.appendInt8 (position);
  return write (message).isValid ();
}

bool Interface::setGripperGroup95 (const Position position)
{ Message message = createMessage (SET_GRIPPER_GROUP_95);
  message.appendInt8 (position);
  return write (message).isValid ();
}

bool Interface::setGripperGroupHeatSealer (const Position position)
{ Message message = createMessage (SET_GRIPPER_GROUP_HEAT_SEALER);
  message.appendInt8 (position);
  return write (message).isValid ();
}

bool Interface::setDoorOpen (void)
{ return write (createMessage (SET_DOOR_OPEN)).isValid ();
}

int Interface::sendGripper485 (const int select, const int address, const int function, const int regAddress, const int data)
{ // Mystery delay to make stuff work.
  QThread::msleep (50);

  Message message = createMessage (SEND_GRIPPER_485);
  message.appendInt8 (select);
  message.appendInt8 (address);
  message.appendInt8 (function);
  message.appendInt16 (regAddress);
  message.appendInt8 (data >> 8);
  message.appendInt8 (data);

  Message response = write (message);
  if (!response.isValid ()) m_log->error ("Interface", "sendGripper485");

  return (response.isValid ()?(static_cast<uint8_t> (response.getInt8 (3)) << 8) + static_cast<uint8_t> (response.getInt8 (4)):-1);
}

bool Interface::set65HeaterGains (const double kpOuter, const double kiOuter, const double kpInner, const double kiInner, const double rateGain)
{ Message message = createMessage (SET_65_HEATER_GAINS);
  message.appendFloat (kpOuter);
  message.appendFloat (kiOuter);
  message.appendFloat (kpInner);
  message.appendFloat (kiInner);
  message.appendFloat (rateGain);
  return write (message).isValid ();
}

Interface::HeaterGains Interface::get65HeaterGains (void)
{ return HeaterGains (write (createMessage (GET_65_HEATER_GAINS)));
}

bool Interface::set95HeaterGains (const double kpOuter, const double kiOuter, const double kpInner, const double kiInner, const double rateGain)
{ Message message = createMessage (SET_95_HEATER_GAINS);
  message.appendFloat (kpOuter);
  message.appendFloat (kiOuter);
  message.appendFloat (kpInner);
  message.appendFloat (kiInner);
  message.appendFloat (rateGain);
  return write (message).isValid ();
}

Interface::HeaterGains Interface::get95HeaterGains (void)
{ return HeaterGains (write (createMessage (GET_95_HEATER_GAINS)));
}

bool Interface::setHeatSealerGains (const double kpOuter, const double kiOuter, const double kpInner, const double kiInner, const double rateGain)
{ Message message = createMessage (SET_HEAT_SEALER_GAINS);
  message.appendFloat (kpOuter);
  message.appendFloat (kiOuter);
  message.appendFloat (kpInner);
  message.appendFloat (kiInner);
  message.appendFloat (rateGain);
  return write (message).isValid ();
}

Interface::HeaterGains Interface::getHeatSealerGains (void)
{ return HeaterGains (write (createMessage (GET_HEAT_SEALER_GAINS)));
}

bool Interface::setTempControlStream (const Channel channel, const int divisor)
{ Message message = createMessage (SET_TEMP_CONTROL_STREAM);
  message.appendInt8 (channel);
  message.appendInt8 (divisor);
  return write (message).isValid ();
}

bool Interface::setRtdTempStream (const int divisor)
{ Message message = createMessage (SET_RTD_TEMP_STREAM);
  message.appendInt8 (divisor);
  return write (message).isValid ();
}

bool Interface::setThermistorTempStream (const int divisor)
{ Message message = createMessage (SET_THERMISTOR_TEMP_STREAM);
  message.appendInt8 (divisor);
  return write (message).isValid ();
}

bool Interface::setRunDataStream (const int divisor)
{ Message message = createMessage (SET_RUN_DATA_STREAM);
  message.appendInt8 (divisor);
  return write (message).isValid ();
}

bool Interface::setSchedulerStatusStream (const int divisor)
{ Message message = createMessage (SET_SCHEDULER_STATUS_STREAM);
  message.appendInt8 (divisor);
  return write (message).isValid ();
}

bool Interface::setTempLogsStream (const int divisor)
{ Message message = createMessage (SET_TEMP_LOGS_STREAM);
  message.appendInt8 (divisor);
  return write (message).isValid ();
}

QList<Interface::Temperature> Interface::getRtdTemps (void)
{ return Temperature::temperatures (write (createMessage (GET_RTD_TEMPS)));
}

QList<Interface::Temperature> Interface::getThermistorTemps (void)
{ return Temperature::temperatures (write (createMessage (GET_THERMISTOR_TEMPS)));
}

Interface::AnalogMonitor Interface::getAnalogMonitor (void)
{ return AnalogMonitor (write (createMessage (GET_ANALOG_MONITOR)));
}

QList<QString> Interface::listDevices (void)
{ QList<QString> devices;

  for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts ())
    if ((info.vendorIdentifier () == vendorId) && (info.productIdentifier () == productId))
      devices.append (info.portName ());

  return devices;
}

void Interface::packetReceived (const QByteArray &packet)
{ Message message (packet);

  //m_log->debug ("Interface", "RX: " + QString (packet.toHex ()));

  if (message.id () == m_responseId)
  { if ((message.type () == m_responseType) || (message.type () == 0))
    { m_response = message;
    } else
    { m_response = Message ();
      switch (message.type ())
      { case COMMAND_ERROR: m_log->error ("Interface", "Command error."); break;
        case CRC_ERROR:     m_log->error ("Interface", "CRC error."); break;
        default:            m_log->error ("Interface", "Unexpected response received.");
      }
    }

    m_responseReceived = true;
  } else
  { switch (message.type ())
    { case STREAM_RTD_TEMP_SENSORS:         emit rtdTempStreamReceived (Temperature::temperatures (message)); break;
      case STREAM_THERMISTOR_TEMP_SENSORS:  emit thermistorTempStreamReceived (Temperature::temperatures (message)); break;
      case STREAM_RUN_DATA:                 emit runDataStreamReceived (RunData (message)); break;
      case STREAM_SCHEDULER_STATUS:         emit schedulerStatusStreamReceived (SchedulerStatus (message)); break;
      case STREAM_TEMP_LOGS:                emit tempLogsStreamReceived (TempLogs (message)); break;
      default:                              m_log->error ("Interface", "Unexpected message received.");
    }
  }
}

Message Interface::write (const Message &message)
{ QByteArray data = message.data ();
  //m_log->debug ("Interface", "TX: " + QString (message.data ().toHex ()));

  m_responseType = message.type ();
  m_responseId = message.id ();
  m_responseReceived = false;

  if (!m_slip.write (data)) return Message ();

  QElapsedTimer timer;
  timer.start ();

  while (!m_responseReceived && (timer.elapsed () < 1000))
  { QCoreApplication::processEvents (QEventLoop::ExcludeUserInputEvents);
  }

  return (m_responseReceived?m_response:Message ());
}

Message Interface::createMessage (const int type)
{ if (++m_id == 256) m_id = 1;
  return Message (type, m_id);
}

Interface::AnalogMonitor::AnalogMonitor (const Message &message)
{ //QTextStream (stdout) << "AnalogMonitor: type=" << message.type () << ", len=" << message.length () << Qt::endl;
  if ((message.type () == GET_ANALOG_MONITOR) && (message.length () == 40))
  { m_vMotor = message.getFloat (0);
    m_v24Sw = message.getFloat (4);
    m_v24 = message.getFloat (8);
    m_v5 = message.getFloat (12);
    m_v3p3 = message.getFloat (16);
    m_laserSense = message.getFloat (20);
    m_spindleOpto = message.getFloat (24);
    m_auxLimit1 = message.getFloat (28);
    m_auxLimit2 = message.getFloat (32);
  } else
  { m_vMotor = qSNaN ();
    m_v24Sw = qSNaN ();
    m_v24 = qSNaN ();
    m_v5 = qSNaN ();
    m_v3p3 = qSNaN ();
    m_laserSense = qSNaN ();
    m_spindleOpto = qSNaN ();
    m_auxLimit1 = qSNaN ();
    m_auxLimit2 = qSNaN ();
  }
}

double Interface::AnalogMonitor::vMotor (void) const
{ return m_vMotor;
}

double Interface::AnalogMonitor::v24Sw (void) const
{ return m_v24Sw;
}

double Interface::AnalogMonitor::v24 (void) const
{ return m_v24;
}

double Interface::AnalogMonitor::v5 (void) const
{ return m_v5;
}

double Interface::AnalogMonitor::v3p3 (void) const
{ return m_v3p3;
}

double Interface::AnalogMonitor::laserSense (void) const
{ return m_laserSense;
}

double Interface::AnalogMonitor::spindleOpto (void) const
{ return m_spindleOpto;
}

double Interface::AnalogMonitor::auxLimit1 (void) const
{ return m_auxLimit1;
}

double Interface::AnalogMonitor::auxLimit2 (void) const
{ return m_auxLimit2;
}

Interface::AmcEncoder::AmcEncoder (const Message &message)
{ if ((message.type () == AMC_GET_ENCODER) && (message.length () == 6))
  { m_encoderCounts = message.getUint16 (0);
    m_indexCapture = message.getUint16 (2);
    m_out1 = message.getUint8 (4);
    m_out2 = message.getUint8 (5);
  } else
  { m_encoderCounts = -1;
    m_indexCapture = -1;
    m_out1 = -1;
    m_out2 = -1;
  }
}

int Interface::AmcEncoder::encoderCounts (void) const
{ return m_encoderCounts;
}

int Interface::AmcEncoder::indexCapture (void) const
{ return m_indexCapture;
}

int Interface::AmcEncoder::out1 (void) const
{ return m_out1;
}

int Interface::AmcEncoder::out2 (void) const
{ return m_out2;
}

Interface::HeaterGains::HeaterGains (const Message &message)
{ if (message.length () == 32)
  { m_kpOuterTop = message.getFloat (0);
    m_kiOuterTop = message.getFloat (4);
    m_kpInnerTop = message.getFloat (8);
    m_kiInnerTop = message.getFloat (12);
    m_kpOuterBottom = message.getFloat (16);
    m_kiOuterBottom = message.getFloat (20);
    m_kpInnerBottom = message.getFloat (24);
    m_kiInnerBottom = message.getFloat (28);
  } else
  { m_kpOuterTop = qSNaN ();
    m_kiOuterTop = qSNaN ();
    m_kpInnerTop = qSNaN ();
    m_kiInnerTop = qSNaN ();
    m_kpOuterBottom = qSNaN ();
    m_kiOuterBottom = qSNaN ();
    m_kpInnerBottom = qSNaN ();
    m_kiInnerBottom = qSNaN ();
  }
}

double Interface::HeaterGains::kpOuterTop (void) const
{ return m_kpOuterTop;
}

double Interface::HeaterGains::kiOuterTop (void) const
{ return m_kiOuterTop;
}

double Interface::HeaterGains::kpInnerTop (void) const
{ return m_kpInnerTop;
}

double Interface::HeaterGains::kiInnerTop (void) const
{ return m_kiInnerTop;
}

double Interface::HeaterGains::kpOuterBottom (void) const
{ return m_kpOuterBottom;
}

double Interface::HeaterGains::kiOuterBottom (void) const
{ return m_kiOuterBottom;
}

double Interface::HeaterGains::kpInnerBottom (void) const
{ return m_kpInnerBottom;
}

double Interface::HeaterGains::kiInnerBottom (void) const
{ return m_kiInnerBottom;
}

Interface::RunData::RunData (const Message &message)
{ if ((message.type () == STREAM_RUN_DATA) && (message.length () == 33))
  { m_schedulerRunning = message.getBool (0);
    m_schedulerIndex = message.getInt32 (1);
    m_spindleAngle = message.getInt32 (5);
    m_gantryPosition = message.getInt32 (9);
    m_temperature65Top = message.getFloat (13);
    m_temperature65Bottom = message.getFloat (17);
    m_temperature95Top = message.getFloat (21);
    m_temperature95Bottom = message.getFloat (25);
    m_temperatureHeatSealer = message.getFloat (29);
  } else
  { m_schedulerRunning = false;
    m_schedulerIndex = -1;
    m_spindleAngle = 0;
    m_gantryPosition = 0;
    m_temperature65Top = qSNaN ();
    m_temperature65Bottom = qSNaN ();
    m_temperature95Top = qSNaN ();
    m_temperature95Bottom = qSNaN ();
    m_temperatureHeatSealer = qSNaN ();
  }
}

bool Interface::RunData::schedulerRunning (void) const
{ return m_schedulerRunning;
}

int Interface::RunData::schedulerIndex (void) const
{ return m_schedulerIndex;
}

int Interface::RunData::spindleAngle (void) const
{ return m_spindleAngle;
}

int Interface::RunData::gantryPosition (void) const
{ return m_gantryPosition;
}

double Interface::RunData::temperature65Top (void) const
{ return m_temperature65Top;
}

double Interface::RunData::temperature65Bottom (void) const
{ return m_temperature65Bottom;
}

double Interface::RunData::temperature95Top (void) const
{ return m_temperature95Top;
}

double Interface::RunData::temperature95Bottom (void) const
{ return m_temperature95Bottom;
}

double Interface::RunData::temperatureHeatSealer (void) const
{ return m_temperatureHeatSealer;
}

Interface::SchedulerStatus::SchedulerStatus (const Message &message)
{ if (((message.type () == SCHEDULER_GET_STATUS) || (message.type () == STREAM_SCHEDULER_STATUS)) && (message.length () == 28))
  { m_runningFlag = message.getBool (0);
    m_errorFlag = message.getBool (1);
    m_errorStep = message.getInt32 (2);
    m_currentStep = message.getInt32 (6);
    m_totalSteps = message.getInt32 (10);
    m_remainingSpindleBuffer = message.getUint16 (14);
    m_remainingHeaterBuffer = message.getUint16 (16);
    m_remainingGripperBuffer = message.getUint16 (18);
    m_remainingStepperBuffer = message.getUint16 (20);
    m_remainingDoorBuffer = message.getUint16 (22);
    m_remainingTimerBuffer = message.getUint16 (24);
    m_remainingLoopBuffer = message.getUint16 (26);
  } else
  { m_runningFlag = false;
    m_errorFlag = true;
    m_errorStep = -1;
    m_currentStep = -1;
    m_totalSteps = -1;
    m_remainingSpindleBuffer = -1;
    m_remainingHeaterBuffer = -1;
    m_remainingGripperBuffer = -1;
    m_remainingStepperBuffer = -1;
    m_remainingDoorBuffer = -1;
    m_remainingTimerBuffer = -1;
    m_remainingLoopBuffer = -1;
  }
}

bool Interface::SchedulerStatus::runningFlag (void) const
{ return m_runningFlag;
}

bool Interface::SchedulerStatus::errorFlag (void) const
{ return m_errorFlag;
}

int Interface::SchedulerStatus::errorStep (void) const
{ return m_errorStep;
}

int Interface::SchedulerStatus::currentStep (void) const
{ return m_currentStep;
}

int Interface::SchedulerStatus::totalSteps (void) const
{ return m_totalSteps;
}

int Interface::SchedulerStatus::remainingSpindleBuffer (void) const
{ return m_remainingSpindleBuffer;
}

int Interface::SchedulerStatus::remainingHeaterBuffer (void) const
{ return m_remainingHeaterBuffer;
}

int Interface::SchedulerStatus::remainingGripperBuffer (void) const
{ return m_remainingGripperBuffer;
}

int Interface::SchedulerStatus::remainingStepperBuffer (void) const
{ return m_remainingStepperBuffer;
}

int Interface::SchedulerStatus::remainingDoorBuffer (void) const
{ return m_remainingDoorBuffer;
}

int Interface::SchedulerStatus::remainingTimerBuffer (void) const
{ return m_remainingTimerBuffer;
}

int Interface::SchedulerStatus::remainingLoopBuffer (void) const
{ return m_remainingLoopBuffer;
}

Interface::StepperState::StepperState (const Message &message)
{ if ((message.type () == GET_STEPPER_STATE) && (message.length () == 2))
  { m_mode = message.getUint8 (0);
    m_homeState = message.getUint8 (1);
  } else
  { m_mode = - 1;
    m_homeState = -1;
  }
}

int Interface::StepperState::mode (void) const
{ return m_mode;
}

int Interface::StepperState::homeState (void) const
{ return m_homeState;
}

QList<Interface::Temperature> Interface::Temperature::temperatures (const Message &message)
{ QList<Temperature> result;

  int count = message.length () / 16;
  for (int index = 0; index < count; index++)
    result.append (Temperature (message, index * 16));

  return result;
}

bool Interface::Temperature::isValid (void) const
{ return m_isValid;
}

double Interface::Temperature::temperature (void) const
{ return m_temperature;
}

double Interface::Temperature::rate (void) const
{ return m_rate;
}

int Interface::Temperature::lastRead (void) const
{ return m_lastRead;
}

Interface::Temperature::Temperature (const Message &message, const int offset)
{ m_isValid = message.getInt32 (offset) != 0;
  m_temperature = message.getFloat (offset + 4);
  m_rate = message.getFloat (offset + 8);
  m_lastRead = message.getInt32 (offset + 12);
}

Interface::TempLogs::TempLogs (const Message &message)
{ if ((message.type () == STREAM_TEMP_LOGS) && (message.length () >= 52))
  { m_rtd65Top1 = message.getFloat (0);
    m_rtd65Top2 = message.getFloat (4);
    m_thermistor65Top = message.getFloat (8);
    m_rtd65Bottom1 = message.getFloat (12);
    m_rtd65Bottom2 = message.getFloat (16);
    m_thermistor65Bottom = message.getFloat (20);
    m_rtd95Top1 = message.getFloat (24);
    m_rtd95Top2 = message.getFloat (28);
    m_thermistor95Top = message.getFloat (32);
    m_rtd95Bottom1 = message.getFloat (36);
    m_rtd95Bottom2 = message.getFloat (40);
    m_thermistor95Bottom = message.getFloat (44);
    m_heatSealerThermistor = message.getFloat (48);
  } else
  { m_rtd65Top1 = qSNaN ();
    m_rtd65Top2 = qSNaN ();
    m_thermistor65Top = qSNaN ();
    m_rtd65Bottom1 = qSNaN ();
    m_rtd65Bottom2 = qSNaN ();
    m_thermistor65Bottom = qSNaN ();
    m_rtd95Top1 = qSNaN ();
    m_rtd95Top2 = qSNaN ();
    m_thermistor95Top = qSNaN ();
    m_rtd95Bottom1 = qSNaN ();
    m_rtd95Bottom2 = qSNaN ();
    m_thermistor95Bottom = qSNaN ();
    m_heatSealerThermistor = qSNaN ();
  }
}

double Interface::TempLogs::rtd65Top1 (void) const
{ return m_rtd65Top1;
}

double Interface::TempLogs::rtd65Top2 (void) const
{ return m_rtd65Top2;
}

double Interface::TempLogs::thermistor65Top (void) const
{ return m_thermistor65Top;
}

double Interface::TempLogs::rtd65Bottom1 (void) const
{ return m_rtd65Bottom1;
}

double Interface::TempLogs::rtd65Bottom2 (void) const
{ return m_rtd65Bottom2;
}

double Interface::TempLogs::thermistor65Bottom (void) const
{ return m_thermistor65Bottom;
}

double Interface::TempLogs::rtd95Top1 (void) const
{ return m_rtd95Top1;
}

double Interface::TempLogs::rtd95Top2 (void) const
{ return m_rtd95Top2;
}

double Interface::TempLogs::thermistor95Top (void) const
{ return m_thermistor95Top;
}

double Interface::TempLogs::rtd95Bottom1 (void) const
{ return m_rtd95Bottom1;
}

double Interface::TempLogs::rtd95Bottom2 (void) const
{ return m_rtd95Bottom2;
}

double Interface::TempLogs::thermistor95Bottom (void) const
{ return m_thermistor95Bottom;
}

double Interface::TempLogs::heatSealerThermistor (void) const
{ return m_heatSealerThermistor;
}
