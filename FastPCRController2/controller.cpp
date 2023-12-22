#include <QCoreApplication>
#include <QDir>
#include <QJsonDocument>
#include "../optics/cuvetteintensities.h"
#include "controller.h"

const int Controller::laserChannel = 4;

Controller::Controller (Log *log, QObject *parent) : QObject (parent), m_log (log), m_cycler (log), m_optics (log)
{ m_state = NotReady;
  m_hasProtocol = false;

  m_isThermocycling = false;
  m_inductionActive = false;

  m_ledMask = 0;

  QList<CaptureItem> items;
  items.append (CaptureItem ());
  m_captureConfig.setItems (items);

  m_inductionTimer.setSingleShot (true);
  m_laserTimer.setSingleShot (true);

  connect (log, &Log::logReceived, this, &Controller::logReceived);
  connect (&m_cycler, &Cycler::stateChanged, this, &Controller::cyclerStateChanged);
  connect (&m_cycler, &Cycler::temperaturesReceived, this, &Controller::cyclerTemperaturesReceived);
  connect (&m_cycler, &Cycler::started, this, &Controller::cyclerStarted);
  connect (&m_cycler, &Cycler::progressed, this, &Controller::cyclerProgressed);
  connect (&m_optics, &OpticsDevice::dataReceived, this, &Controller::opticsDataReceived);
  connect (&m_optics, &OpticsDevice::encoderReceived, this, &Controller::opticsEncoderReceived);
  connect (&m_inductionTimer, &QTimer::timeout, this, &Controller::inductionTimeout);
  connect (&m_laserTimer, &QTimer::timeout, this, &Controller::laserTimeout);
}

Controller::~Controller (void)
{ closeCycler ();
  closeInduction ();
  closeOptics ();

  m_writer.close ();
}

bool Controller::isOpenCycler (void) const
{ return m_cycler.isOpen ();
}

bool Controller::isOpenInduction (void) const
{ return m_induction.isOpen ();
}

bool Controller::isOpenOptics (void) const
{ return m_optics.isOpen ();
}

bool Controller::openCycler (void)
{ if (m_cycler.open ())
  { checkReady ();
    return true;
  }

  return false;
}

bool Controller::openInduction (const QString &portName)
{ if (m_induction.open (portName))
  { checkReady ();
    return true;
  }

  return false;
}

bool Controller::openOptics (void)
{ QList<UsbDeviceInfo> devices = OpticsDevice::listDevices ();
  if (!devices.isEmpty ())
  { if (m_optics.open (devices.first ()))
    { if (configureLeds ())
      { if (m_optics.captureConfigure (m_captureConfig))
        { if (m_optics.captureEnable ())
          { m_diskIndexEnabled = false;
            return true;
          }
        }
      }
    }
  }

  return false;
}

void Controller::closeCycler (void)
{ m_cycler.close ();
  checkReady ();
}

void Controller::closeInduction (void)
{ if (m_inductionActive)
    if (m_induction.stopPwm ())
      m_inductionActive = false;

  m_induction.close ();
  checkReady ();
}

void Controller::closeOptics (void)
{ m_optics.close ();
}

bool Controller::needInduction (void) const
{ return m_protocol.samplePrepConfig ().enable ();
}

bool Controller::hasProtocol (void) const
{ return m_hasProtocol;
}

PcrProtocol Controller::protocol (void) const
{ return m_protocol;
}

bool Controller::setProtocol (const PcrProtocol &protocol)
{ if ((m_state == Running) || (m_state == PcrPaused) || (m_state == PreHbhPaused)) return false;

  m_hasProtocol = true;
  m_protocol = protocol;
  checkReady ();

  return true;
}

DiskConfiguration Controller::diskConfig (void) const
{ return m_diskConfig;
}

void Controller::setDiskConfig (const DiskConfiguration &config)
{ m_diskConfig = config;
}

LedConfigurationContainer Controller::ledConfig (void) const
{ return m_ledConfig;
}

int Controller::ledEnable (void) const
{ return m_ledMask;
}

bool Controller::setLedConfig (const LedConfigurationContainer &config)
{ if ((m_state == Running) || (m_state == PcrPaused) || (m_state == PreHbhPaused)) return false;
  m_ledConfig = config;
  return configureLeds ();
}

bool Controller::setLedEnable (const int ledMask)
{ if ((m_state == Running) || (m_state == PcrPaused) || (m_state == PreHbhPaused)) return false;
  m_ledMask = ledMask;
  return true;
}

bool Controller::setSealerEnable (const int sealerMask)
{ if ((m_state == Running) || (m_state == PcrPaused) || (m_state == PreHbhPaused)) return false;
  m_protocol.setSealerEnable(sealerMask);
  return true;
}

CuvetteLocationConfigurationContainer Controller::cuvetteConfig (void) const
{ return m_cuvetteConfig;
}

void Controller::setCuvetteConfig (const CuvetteLocationConfigurationContainer &config)
{ m_cuvetteConfig = config;
}

CuvetteProcessingConfiguration Controller::processingConfig (void) const
{ return m_processingConfig;
}

void Controller::setProcessingConfig (const CuvetteProcessingConfiguration &config)
{ m_processingConfig = config;
}

bool Controller::start (const QString &directory, const QString &fileNameSuffix, const bool runPreHbh, const bool pcrPause, const bool preHbhPause, const UserInputData &userInput)
{ if (m_state == Ready)
  { if (openWriter (directory, fileNameSuffix, userInput))
    { // if (m_optics.encoderStart (0.001))
      { if (m_cycler.start (m_protocol, m_diskConfig, runPreHbh, pcrPause, preHbhPause))
        { m_profiles.clear ();
          m_elapsedTimer.start ();

          changeState (Running);
          return true;
        }
      } // else m_optics.encoderStop ();

      m_writer.close ();
    }
  }

  return false;
}

bool Controller::resume (void)
{ if ((m_state == PcrPaused) || (m_state == PreHbhPaused))
  { if (m_cycler.resume ())
    { changeState (Running);
      return true;
    }
  }

  return false;
}

void Controller::abort (void)
{ if ((m_state == Running) || (m_state == PcrPaused) || (m_state == PreHbhPaused))
  { m_isThermocycling = false;
    updateLeds ();
    m_induction.stopPwm ();
    //m_optics.laserOff ();
    m_cycler.abort ();
    m_optics.encoderStop ();
    m_writer.close ();

    changeState (Ready);
  }
}

bool Controller::standby65On (const double temperature)
{ return m_cycler.heater65On (temperature);
}

bool Controller::standby95On (const double temperature)
{ return m_cycler.heater95On (temperature);
}

bool Controller::standbySealerOn (const double temperature)
{ return m_cycler.heatSealerOn (temperature);
}

bool Controller::standby65Off (void)
{ return m_cycler.heater65Off ();
}

bool Controller::standby95Off (void)
{ return m_cycler.heater95Off ();
}

bool Controller::standbySealerOff (void)
{ return m_cycler.heatSealerOff ();
}

void Controller::logReceived (const QDateTime &timestamp, const Log::Severity severity, const QString &source, const QString &text)
{ m_writer.writeLog (timestamp, severity, source, text);
}

void Controller::cyclerStateChanged (const Cycler::State newState)
{ if ((newState == Cycler::Ready) && ((m_state == Running) || (m_state == PcrPaused) || (m_state == PreHbhPaused)))
  { abort ();
  }
}

void Controller::cyclerTemperaturesReceived (const QVector<double> &top65, const QVector<double> &bottom65, const QVector<double> &top95, const QVector<double> &bottom95, const double heatSealer)
{ m_writer.writeTemperatures (QDateTime::currentDateTime (), top65, bottom65, top95, bottom95, heatSealer);
  emit temperaturesReceived (top65, bottom65, top95, bottom95, heatSealer);
}

void Controller::cyclerStarted (const int steps)
{ emit started (steps);
}

void Controller::cyclerProgressed (const int step, const Progress &progress)
{ bool diskIndexEnabled = (progress.subState () == Progress::Homing);
  bool isThermocycling = (progress.state () == Progress::Thermocycling);
  Valve valve;

  emit progressed (step, progress);

  if (m_diskIndexEnabled != diskIndexEnabled)
  { bool result = (diskIndexEnabled?m_optics.diskIndexEnable ():m_optics.diskIndexDisable ());
    if (result) m_diskIndexEnabled = diskIndexEnabled;
  }

  if (m_isThermocycling != isThermocycling)
  { m_isThermocycling = isThermocycling;
    updateLeds ();
  }

  //printf("Controller:cyclerProgressed, m_state=%d\n", m_state);
  if (m_state == Running)
  { if (progress.state () == Progress::PcrPausing)
    { changeState (PcrPaused);
    }
    else if (progress.state () == Progress::PreHbhPausing)
    { changeState (PreHbhPaused);
    }
    else
    { switch (progress.subState ())
      { case Progress::InductionHeating:
          /*** T.B.D.: Turn on induction PWM. ***/
          m_log->warning ("Controller", "****************** Induction on (" + QString::number (m_protocol.samplePrepConfig ().heatingDuration (), 'f', 1) + ") ******************");
          m_inductionActive = true;
          m_inductionTimer.start (qRound (m_protocol.samplePrepConfig ().heatingDuration () * 1000.0));
          break;

        case Progress::FiringLaser:
          switch (progress.state ())
          { case Progress::SamplePrep: valve = m_diskConfig.hyperbaricValve (); break;
            case Progress::Metering: valve = m_diskConfig.meteringValve (); break;
            case Progress::Resuspension: valve = m_diskConfig.insertValve (); break;
            default: break;
          }

          m_log->warning ("Controller", "****************** Laser on (" + QString::number (valve.duration (), 'f', 1) + ") ******************");
          m_laserActive = true;

          /*** T.B.D.: Take shunt configuration from UI. ***/
          m_optics.ledConfigure (laserChannel, LedConfiguration (valve.current (), 0.1));
          m_optics.ledOn (1 << 4, valve.duration ());

          m_laserTimer.start (qRound (valve.duration () * 1000.0) + 500);
          break;

        default:
          break;
        }
    }
  }
}

void Controller::opticsDataReceived (const RawData &data, const CaptureConfiguration &config)
{ m_writer.writeRawData (data);

  QList<TimeData> timeData = TimeData::process (data, config);
  QList<SpatialData> spatialData = SpatialData::process (timeData);

  emit timeDataProcessed (timeData);
  emit spatialDataProcessed (spatialData);

  for (const SpatialData &data : spatialData)
  { CuvetteIntensities intensities (data, m_cuvetteConfig.configuration (data.channel ()), m_processingConfig);
    QList<double> values = intensities.values ();

    for (int cuvette = 0; cuvette < intensities.numOfCuvettes (); cuvette++)
    { CuvetteId id (data.channel (), cuvette);
      if (!m_profiles.contains (id)) m_profiles.insert (id, PcrProfile (id));
      m_profiles [id].append (values.at (cuvette));
    }
  }

  emit cycleCompleted (m_profiles.values ());
}

void Controller::opticsEncoderReceived (const QByteArray &data)
{ m_writer.writeEncoder (data);
}

void Controller::inductionTimeout (void)
{ m_log->warning ("Controller", "****************** Induction off ******************");
  //if (m_induction.stopPwm ())
  { m_inductionActive = false;
    m_cycler.resume ();
  }
}

void Controller::laserTimeout (void)
{ m_log->warning ("Controller", "****************** Laser off ******************");
  m_laserActive = false;
  m_cycler.resume ();
}

void Controller::checkReady (void)
{ //bool ready = m_cycler.isOpen () && m_hasProtocol && (!m_protocol.samplePrepConfig ().enable () || m_induction.isOpen ()) && m_optics.isOpen ();
  bool ready = m_cycler.isOpen () && m_hasProtocol && m_optics.isOpen ();

  if (ready && (m_state == NotReady))
  { changeState (Ready);
  } else
  if (!ready && (m_state == Ready))
  { changeState (NotReady);
  }
}

// Ming: for the test tab. Bypass checking protocol and optics
void Controller::stepperCheckReady (void)
{
  bool ready = m_cycler.isOpen();

  if (ready && (m_state == NotReady))
  { changeState (Ready);
  } else
    if (!ready && (m_state == Ready))
    { changeState (NotReady);
    }
}


void Controller::changeState (const State newState)
{ m_state = newState;
  emit stateChanged (newState);
}

bool Controller::updateLeds (void)
{ if (m_isThermocycling) return m_optics.ledOn (m_ledMask);
  return m_optics.ledOff (m_ledMask);
}

bool Controller::configureLeds (void)
{ for (const int channel : m_ledConfig.channels ())
    if (!m_optics.ledConfigure (channel, m_ledConfig.configuration (channel))) return false;
  return true;
}

bool Controller::openWriter (const QString &directory, const QString &fileNameSuffix, const UserInputData &userInput)
{ QString fileName = (directory.isEmpty ()?QString ():directory + QDir::separator ()) + "pcr_" + QDateTime::currentDateTime ().toString ("yyyy_MM_dd_hh_mm_ss_zzz") + (fileNameSuffix.isEmpty ()?QString ():"_" + fileNameSuffix) + ".pcr";

  if (m_writer.open (fileName))
  { if (m_writer.writeCaptureConfiguration (m_captureConfig))
    { if (m_writer.writeCuvetteLocationConfiguration (m_cuvetteConfig))
      { if (m_writer.writePcrProtocol (m_protocol))
        { if (m_writer.writeUserInput (userInput))
          { if (writeGripperConfig ())
            { return true;
            }
          }
        }
      }
    }

    m_writer.close ();
  }

  return false;
}

bool Controller::writeGripperConfig (void)
{ const Pge::Select select [] = {Pge::Heater65, Pge::Heater95, Pge::HeatSealer};
  const Pge::IoGroup ioGroup [] = {Pge::Group1, Pge::Group2, Pge::Group3, Pge::Group4};

  for (int y = 0; y < 3; y++)
    for (int x = 0; x < 4; x++)
        if (!m_writer.writeGripperConfiguration (select [y], ioGroup [x], m_cycler.gripperPosition (select [y], ioGroup [x]), m_cycler.gripperForce (select [y], ioGroup [x]), m_cycler.gripperSpeed (select [y], ioGroup [x]))) return false;

  return true;
}

bool Controller::stepperHome (void)
{
  // To check and set the state to be in Ready
  stepperCheckReady();

  if (m_state != Ready) return false;
  if (!m_cycler.stepperHome ()) return false;
  return true;
}

bool Controller::stepperRetract (void)
{
  // To check and set the state to be in Ready
  stepperCheckReady();

  if (m_state != Ready) return false;
  if (!m_cycler.stepperRetract ()) return false;
  return true;
}

bool Controller::stepperMovePositionRelative (const int steps, const double maxSpeed)
{
  // To check and set the state to be in Ready
  stepperCheckReady();

  if (m_state != Ready) return false;
  if (m_cycler.stepperMovePositionRelative (steps, maxSpeed)) return false;
  return true;
}

bool Controller::stepperMovePositionAbsolute (const int steps, const double maxSpeed)
{
  // To check and set the state to be in Ready
  stepperCheckReady();

  if (m_state != Ready) return false;
  if (!m_cycler.stepperMovePositionAbsolute (steps, maxSpeed)) return false;
  return true;
}

bool Controller::setSealerClose (void)
{
  // To check and set the state to be in Ready
  stepperCheckReady();

  if (m_state != Ready) return false;
  if (!m_cycler.setGripperGroupHeatSealer(Interface::Closed)) return false;
  return true;
}

bool Controller::setSealerOpen (void)
{
  // To check and set the state to be in Ready
  stepperCheckReady();

  if (m_state != Ready) return false;
  if (!m_cycler.setGripperGroupHeatSealer(Interface::FullOpen)) return false;
  return true;
}

bool Controller::setHeater65Close (void)
{
  // To check and set the state to be in Ready
  stepperCheckReady();

  if (m_state != Ready) return false;
  if (!m_cycler.setGripperGroup65(Interface::Closed)) return false;

  return true;
}

bool Controller::setHeater65Open (void)
{
  // To check and set the state to be in Ready
  stepperCheckReady();

  if (m_state != Ready) return false;
  if (!m_cycler.setGripperGroup65(Interface::FullOpen)) return false;

  return true;
}

bool Controller::setHeater95Close (void)
{
  // To check and set the state to be in Ready
  stepperCheckReady();

  if (m_state != Ready) return false;
  if (!m_cycler.setGripperGroup95(Interface::Closed)) return false;

  return true;
}

bool Controller::setHeater95Open (void)
{
  // To check and set the state to be in Ready
  stepperCheckReady();

  if (m_state != Ready) return false;
  if (!m_cycler.setGripperGroup95(Interface::FullOpen)) return false;

  return true;
}

bool Controller::setHeaterBothClose (void)
{
  // To check and set the state to be in Ready
  stepperCheckReady();

  if (m_state != Ready) return false;
  if (!m_cycler.setGripperGroup65(Interface::Closed)) return false;
  if (!m_cycler.setGripperGroup95(Interface::Closed)) return false;

  return true;
}

bool Controller::setHeaterBothOpen (void)
{
  // To check and set the state to be in Ready
  stepperCheckReady();
  if (m_state != Ready) return false;

  if (!m_cycler.setGripperGroup65(Interface::FullOpen)) return false;
  if (!m_cycler.setGripperGroup95(Interface::FullOpen)) return false;

  return true;
}

bool Controller::spindleSpinStart(const double velocity, const double accel, const double decel)
{
  // To check and set the state to be in Ready
  stepperCheckReady();
  if (m_state != Ready) return false;

  if (!m_cycler.spindleSpinStart(velocity, accel, decel)) return false;
  return true;
}

bool Controller::spindleStop(void)
{
  // To check and set the state to be in Ready
  stepperCheckReady();
  if (m_state != Ready) return false;

  if (!m_cycler.spindleStop()) return false;
  return true;
}

bool Controller::spindleDiskHome(const Interface::Reference reference)
{
  // To check and set the state to be in Ready
  stepperCheckReady();
  if (m_state != Ready) return false;

  if (!m_cycler.spindleDiskHome(Interface::Disk)) return false;
  return true;
}

bool Controller::setSealerOn(const double temperature)
{
  // To check and set the state to be in Ready
  stepperCheckReady();
  if (m_state != Ready) return false;

  if (!m_cycler.heatSealerOn(temperature)) return false;
  return true;
}

bool Controller::setSealerOff(void)
{
  // To check and set the state to be in Ready
  stepperCheckReady();
  if (m_state != Ready) return false;

  if (!m_cycler.heatSealerOff()) return false;
  return true;
}

bool Controller::setHeater65On(const double temperature)
{
  // To check and set the state to be in Ready
  stepperCheckReady();
  if (m_state != Ready) return false;

  if (!m_cycler.heater65On(temperature)) return false;
  return true;
}

bool Controller::setHeater65Off(void)
{
  // To check and set the state to be in Ready
  stepperCheckReady();
  if (m_state != Ready) return false;

  if (!m_cycler.heater65Off()) return false;
  return true;
}

bool Controller::setHeater95On(const double temperature)
{
  // To check and set the state to be in Ready
  stepperCheckReady();
  if (m_state != Ready) return false;

  if (!m_cycler.heater95On(temperature)) return false;
  return true;
}

bool Controller::setHeater95Off(void)
{
  // To check and set the state to be in Ready
  stepperCheckReady();
  if (m_state != Ready) return false;

  if (!m_cycler.heater95Off()) return false;
  return true;
}

bool Controller::amcUartDisable (void)
{ return m_cycler.amcUartDisable();
}

void Controller::ledOnEngineering(const int channel, double current, double shunt)
{ // Set the current and default shunt = 0.1
  m_optics.ledConfigure (channel, LedConfiguration(current, shunt));
  m_optics.ledOn (1 << channel);
}

void Controller::ledOffEngineering(const int channel)
{ m_optics.ledOff (1 << channel);
}
