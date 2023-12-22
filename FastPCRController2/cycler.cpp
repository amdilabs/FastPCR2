#include <QThread>
#include "scheduler.h"
#include "cycler.h"

const double Cycler::pcrHeaterRate = 100.0;
const double Cycler::heatSealerRate = 100.0;

Cycler::Cycler (Log *log, QObject *parent) : QObject (parent), m_log (log), m_interface (log), m_pge (&m_interface)
{ m_state = Closed;

  m_temperature65 = qSNaN ();
  m_temperature95 = qSNaN ();
  m_temperatureHeatSealer = qSNaN ();

  connect (&m_interface, &Interface::runDataStreamReceived, this, &Cycler::runDataStreamReceived);
  connect (&m_interface, &Interface::tempLogsStreamReceived, this, &Cycler::tempLogsStreamReceived);
}

Cycler::~Cycler (void)
{ close ();
}

bool Cycler::open (void)
{ close ();

  QList<QString> devices = Interface::listDevices ();
  if (devices.isEmpty ()) return false;

  if (!m_interface.open (devices.first ())) return false;

  m_version = m_interface.getVersion ();
  if (m_version.isEmpty ()) { m_interface.close (); return false; }
  if (!m_interface.setRunDataStream (40)) { m_interface.close (); return false; }
  if (!m_interface.setTempLogsStream (20)) { m_interface.setRunDataStream (0); m_interface.close (); return false; }
  if (!setHeaters ()) { m_interface.setTempLogsStream (0); m_interface.setRunDataStream (0); m_interface.close (); return false; }

  m_schedulerIndex = -1;
  m_schedulerRunning = false;
//  m_interface.schedulerStop ();

  changeState (Ready);
  return true;
}

void Cycler::close (void)
{ if (m_state != Closed)
  { m_interface.setTempLogsStream (0);
    m_interface.setRunDataStream (0);
    m_interface.disableAllHeaters ();
    m_interface.close ();

    m_version = QString ();
    changeState (Closed);
  }
}

bool Cycler::isOpen (void) const
{ return m_state != Closed;
}

QString Cycler::version (void) const
{ return m_version;
}

bool Cycler::resume (void)
{ if (m_state != Running) return false;
  return m_interface.schPauseResume ();
}

bool Cycler::retract (void)
{ if (m_state != Ready) return false;
  return m_interface.stepperHome (450000, 30000, 0);
}

bool Cycler::heater65On (const double temperature)
{ m_temperature65 = temperature;
  if (m_state == Ready) return m_interface.setEnable65Heaters (temperature, pcrHeaterRate);
  return true;
}

bool Cycler::heater95On (const double temperature)
{ m_temperature95 = temperature;
  if (m_state == Ready) return m_interface.setEnable95Heaters (temperature, pcrHeaterRate);
  return true;
}

bool Cycler::heatSealerOn (const double temperature)
{ m_temperatureHeatSealer = temperature;
  if (m_state == Ready) return m_interface.setEnableHeatSealer (temperature, heatSealerRate);
  return true;
}

bool Cycler::heater65Off (void)
{ m_temperature65 = qSNaN ();
  if (m_state == Ready) return m_interface.disable65Heaters ();
  return true;
}

bool Cycler::heater95Off (void)
{ m_temperature95 = qSNaN ();
  if (m_state == Ready) return m_interface.disable95Heaters ();
  return true;
}

bool Cycler::heatSealerOff (void)
{ m_temperatureHeatSealer = qSNaN ();
  if (m_state == Ready) return m_interface.disableHeatSealer ();
  return true;
}

double Cycler::gripperPosition (const Pge::Select select, const Pge::IoGroup ioGroup)
{ return m_pge.getIoGroupPosition (select, ioGroup);
}

double Cycler::gripperForce (const Pge::Select select, const Pge::IoGroup ioGroup)
{ return m_pge.getIoGroupForce (select, ioGroup);
}

double Cycler::gripperSpeed (const Pge::Select select, const Pge::IoGroup ioGroup)
{ return m_pge.getIoGroupSpeed (select, ioGroup);
}

bool Cycler::start (const PcrProtocol &protocol, const DiskConfiguration &diskConfig, const bool runPreHbh, const bool pcrPause, const bool preHbhPause)
{ if (m_state != Ready) return false;

  if (!configure (protocol, diskConfig, runPreHbh, pcrPause, preHbhPause)) return false;  // Ming: stepper not moved yet
  if (!m_interface.schedulerStart ()) return false;    // Ming: stepper moves to home

  m_schedulerIndex = -1;
  m_schedulerRunning = false;

  emit started (m_tracker.total ());

  changeState (Running);
  return true;
}

bool Cycler::abort (void)
{ if (m_state != Running) return false;

  // Stop scheduler.
  if (!m_interface.schedulerStop ()) return false;

  // Stop spindle.
  if (!m_interface.amcMoveAtVelocity (0.0)) return false;

  // Open grippers.
  if (!m_interface.setGripperGroup65 (Interface::FullOpen)) return false;
  if (!m_interface.setGripperGroup95 (Interface::FullOpen)) return false;
  if (!m_interface.setGripperGroupHeatSealer (Interface::FullOpen)) return false;

  // Wait for grippers to start opening before retracting gantry.
  QThread::msleep (500);

  // Retract gantry.
  if (!m_interface.stepperHome (900000, 30000, 0)) return false;

  if (!setHeaters ()) return false;

  // Open door.
  if (!m_interface.setDoorOpen ()) return false;

  changeState (Ready);
  return true;
}

void Cycler::runDataStreamReceived (const Interface::RunData &runData)
{ if (runData.schedulerRunning ())
  { if (m_schedulerIndex != runData.schedulerIndex ())
    { m_schedulerIndex = runData.schedulerIndex ();
      emit progressed (m_schedulerIndex, m_tracker.progress (m_schedulerIndex));
    }
  }

  if (m_schedulerRunning && !runData.schedulerRunning ())
  { m_log->info ("Cycler", "*** Done ***");
    changeState (Ready);
  }

  m_schedulerRunning = runData.schedulerRunning ();
}

void Cycler::tempLogsStreamReceived (const Interface::TempLogs &tempLogs)
{ emit temperaturesReceived (
    QVector<double> () << tempLogs.rtd65Top1 () << tempLogs.rtd65Top2 () << tempLogs.thermistor65Top (),
    QVector<double> () << tempLogs.rtd65Bottom1 () << tempLogs.rtd65Bottom2 () << tempLogs.thermistor65Bottom (),
    QVector<double> () << tempLogs.rtd95Top1 () << tempLogs.rtd95Top2 () << tempLogs.thermistor95Top (),
    QVector<double> () << tempLogs.rtd95Bottom1 () << tempLogs.rtd95Bottom2 () << tempLogs.thermistor95Bottom (),
    tempLogs.heatSealerThermistor ());
}

bool Cycler::setHeaters (void)
{ // Run heaters on standby temperature.
  if (!qIsNaN (m_temperature65))
  { if (!m_interface.setEnable65Heaters (m_temperature65, pcrHeaterRate)) return false;
  } else
  { if (!m_interface.disable65Heaters ()) return false;
  }

  if (!qIsNaN (m_temperature95))
  { if (!m_interface.setEnable95Heaters (m_temperature95, pcrHeaterRate)) return false;
  } else
  { if (!m_interface.disable95Heaters ()) return false;
  }

  if (!qIsNaN (m_temperatureHeatSealer))
  { if (!m_interface.setEnableHeatSealer (m_temperatureHeatSealer, heatSealerRate)) return false;
  } else
  { if (!m_interface.disableHeatSealer ()) return false;
  }

  return true;
}

bool Cycler::configure (const PcrProtocol &protocol, const DiskConfiguration &diskConfig, const bool runPreHbh, const bool pcrPause, const bool preHbhPause)
{ if (!m_interface.schedulerClear ()) return false;
  m_tracker.clear ();

  if (runPreHbh)
  { if (!Scheduler::stepsPreHbh (&m_interface, protocol.preHbhConfig (), &m_tracker)) return false;
    if (preHbhPause) if (!Scheduler::stepsPreHbhPause (&m_interface, &m_tracker)) return false;
  }
  if (!Scheduler::stepsPreTest (&m_interface, protocol.fillingConfig (), protocol.rtConfig (), protocol.thermocyclingConfig (), &m_tracker)) return false;
  if (!Scheduler::stepsSamplePrep (&m_interface, diskConfig, protocol.samplePrepConfig (), &m_tracker)) return false;
  if (!Scheduler::stepsMetering (&m_interface, diskConfig, protocol.samplePrepConfig (), protocol.meteringConfig (), &m_tracker)) return false;
  if (!Scheduler::stepsResuspension (&m_interface, diskConfig, protocol.samplePrepConfig (), protocol.resuspensionConfig (), &m_tracker)) return false;
  if (!Scheduler::stepsFilling (&m_interface, diskConfig, protocol.fillingConfig (), m_temperatureHeatSealer, &m_tracker)) return false;
  if (pcrPause) if (!Scheduler::stepsPcrPause (&m_interface, &m_tracker)) return false;
  if (!Scheduler::stepsRt (&m_interface, diskConfig, protocol.rtConfig (), protocol.thermocyclingConfig (), &m_tracker)) return false;
  if (!Scheduler::stepsThermocycling (&m_interface, diskConfig, protocol.thermocyclingConfig (), &m_tracker)) return false;
  if (!Scheduler::stepsPostTest (&m_interface, m_temperature65, m_temperature95, &m_tracker)) return false;

  return true;
}

void Cycler::changeState (const State newState)
{ m_state = newState;
  emit stateChanged (newState);
}

bool Cycler::stepperHome (void)
{
  if (!m_interface.schedulerClear ()) return false;
  if (!m_interface.schStepperHome (true)) return false;

  /* Ming:
   * - using both schStepperWaitHome() and scheudlerStart()
   *        would crash the program, but grantry runs
   * - if comment out schStepperWaitHome() only, no crash but grantry runs
   * - if comment out schedulerStart() only, no crash no grantry run
   */
  //if (!m_interface.schStepperWaitHome ()) return false;
  if (!m_interface.schedulerStart ()) return false;

  m_schedulerIndex = -1;
  m_schedulerRunning = false;

  //emit started (m_tracker.total ());
  //changeState (Running);

  return true;
}

// Ming: Cycler::retrat is existing, so no need for this one?
bool Cycler::stepperRetract (void)
{
  if (!m_interface.schedulerClear ()) return false;
  if (!m_interface.schStepperHome (false)) return false;
  //if (!m_interface.schStepperWaitHome ()) return false;
  if (!m_interface.schedulerStart ()) return false;

  m_schedulerIndex = -1;
  m_schedulerRunning = false;

  //emit started (m_tracker.total ());
  //changeState (Running);

  return true;
}

bool Cycler::stepperMovePositionRelative (const int steps, const double maxSpeed)
{
  // Is Home at 450000, and Retract at 900000?
  if (!m_interface.schedulerClear ()) return false;
  if (!m_interface.schStepperMovePositionRelative (steps, maxSpeed)) return false;
  if (!m_interface.schedulerStart ()) return false;

  // Get position does not work
  int pos = m_interface.stepperGetPosition();

  m_schedulerIndex = -1;
  m_schedulerRunning = false;

  //emit started (m_tracker.total ());
  //changeState (Running);

  return true;
}

bool Cycler::stepperMovePositionAbsolute (const int steps, const double maxSpeed)
{
  if (!m_interface.schedulerClear ()) return false;
  if (!m_interface.schStepperMovePositionAbsolute (steps, maxSpeed)) return false;
  if (!m_interface.schedulerStart ()) return false;

  m_schedulerIndex = -1;
  m_schedulerRunning = false;

  //emit started (m_tracker.total ());
  //changeState (Running);

  return true;
}

bool Cycler::setGripperGroupHeatSealer (const Interface::Position position)
{
  //if (!m_interface.schedulerClear ()) return false;
  if (!m_interface.setGripperGroupHeatSealer (position)) return false;
  //if (!m_interface.schedulerStart ()) return false;

  m_schedulerIndex = -1;
  m_schedulerRunning = false;

  //emit started (m_tracker.total ());
  //changeState (Running);

  return true;
}

bool Cycler::setGripperGroup65 (const Interface::Position position)
{
  //if (!m_interface.schedulerClear ()) return false;
  if (!m_interface.setGripperGroup65(position)) return false;
  //if (!m_interface.schedulerStart ()) return false;

  m_schedulerIndex = -1;
  m_schedulerRunning = false;

  //emit started (m_tracker.total ());
  //changeState (Running);

  return true;
}

bool Cycler::setGripperGroup95 (const Interface::Position position)
{
  //if (!m_interface.schedulerClear ()) return false;
  if (!m_interface.setGripperGroup95(position)) return false;
  //if (!m_interface.schedulerStart ()) return false;

  m_schedulerIndex = -1;
  m_schedulerRunning = false;

  //emit started (m_tracker.total ());
  //changeState (Running);

  return true;
}

bool Cycler::spindleSpinStart (const double velocity, const double accel, const double decel)
{
  if (!m_interface.schedulerClear ()) return false;
  if (!m_interface.schSpindleSetAccelDecel (accel, decel)) return false;
  if (!m_interface.schSpindleMoveSpeed (velocity)) return false;
  if (!m_interface.schedulerStart ()) return false;
  return true;
}

bool Cycler::spindleStop (void)
{
  if (!m_interface.schedulerClear ()) return false;
  if (!m_interface.schSpindleStop ()) return false;
  if (!m_interface.schedulerStart ()) return false;
  return true;
}

bool Cycler::spindleDiskHome (const Interface::Reference reference)
{
  if (!m_interface.schedulerClear ()) return false;
  //if (!m_interface.schSpindleHome (Interface::Disk)) return false;

  const double defaultAcceleration = 8000.0;
  const double defaultDeceleration = 8000.0;
  if (!m_interface.schSpindleSetAccelDecel (defaultAcceleration, defaultDeceleration)) return false;

  if (!m_interface.schSpindleHome (Interface::Disk)) return false;
  if (!m_interface.schSpindleWaitPosition ()) return false;
  if (!m_interface.schTimerStart1 ()) return false;
  if (!m_interface.schTimerWait1 (500)) return false;

  if (!m_interface.schSpindleHome (Interface::Disk)) return false;
  if (!m_interface.schSpindleWaitPosition ()) return false;
  if (!m_interface.schTimerStart1 ()) return false;
  if (!m_interface.schTimerWait1 (500)) return false;

  if (!m_interface.schedulerStart ()) return false;

  m_schedulerIndex = -1;
  m_schedulerRunning = false;

  return true;
}

bool Cycler::amcUartDisable (void)
{ return m_interface.amcUartDisable();
}

