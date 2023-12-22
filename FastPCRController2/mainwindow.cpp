#include "../../lib/settings.h"
#include "../../lib/ui/serialportselectiondialog.h"
#include "../../lib/ui/userinput/userinputwidgets.h"
#include "../ui/cuvettelocationconfigurationdialog.h"
#include "../ui/cuvetteprocessingconfigurationdialog.h"
#include "diskconfigurationdialog.h"
#include "ledconfigurationdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent), ui (new Ui::MainWindow), m_controller (&m_log)
{ ui->setupUi (this);

  ui->controlStorageFileNameEdit->setValidator (&m_validator);

  ui->controlProgressPreHbhButton->setAttribute (Qt::WA_TransparentForMouseEvents);
  ui->controlProgressPreHbhPausedButton->setAttribute (Qt::WA_TransparentForMouseEvents);
  ui->controlProgressPreTestButton->setAttribute (Qt::WA_TransparentForMouseEvents);
  ui->controlProgressSamplePrepButton->setAttribute (Qt::WA_TransparentForMouseEvents);
  ui->controlProgressMeteringButton->setAttribute (Qt::WA_TransparentForMouseEvents);
  ui->controlProgressResuspensionButton->setAttribute (Qt::WA_TransparentForMouseEvents);
  ui->controlProgressFillingButton->setAttribute (Qt::WA_TransparentForMouseEvents);
  ui->controlProgressPcrPausedButton->setAttribute (Qt::WA_TransparentForMouseEvents);
  ui->controlProgressRtButton->setAttribute (Qt::WA_TransparentForMouseEvents);
  ui->controlProgressThermocyclingButton->setAttribute (Qt::WA_TransparentForMouseEvents);
  ui->controlProgressPostTestButton->setAttribute (Qt::WA_TransparentForMouseEvents);

  m_timeDataChannelBoxes.append (ui->timeDataChannelPd0Box);
  m_timeDataChannelBoxes.append (ui->timeDataChannelPd1Box);
  m_timeDataChannelBoxes.append (ui->timeDataChannelPd2Box);
  m_timeDataChannelBoxes.append (ui->timeDataChannelPd3Box);
  m_spatialDataChannelBoxes.append (ui->spatialDataChannelPd0Box);
  m_spatialDataChannelBoxes.append (ui->spatialDataChannelPd1Box);
  m_spatialDataChannelBoxes.append (ui->spatialDataChannelPd2Box);
  m_spatialDataChannelBoxes.append (ui->spatialDataChannelPd3Box);
  m_pcrGraphChannelBoxes.append (ui->pcrGraphChannelPd0Box);
  m_pcrGraphChannelBoxes.append (ui->pcrGraphChannelPd1Box);
  m_pcrGraphChannelBoxes.append (ui->pcrGraphChannelPd2Box);
  m_pcrGraphChannelBoxes.append (ui->pcrGraphChannelPd3Box);
  m_pcrGraphCuvetteBoxes.append (ui->pcrGraphCuvette0Box);
  m_pcrGraphCuvetteBoxes.append (ui->pcrGraphCuvette1Box);
  m_pcrGraphCuvetteBoxes.append (ui->pcrGraphCuvette2Box);
  m_pcrGraphCuvetteBoxes.append (ui->pcrGraphCuvette3Box);
  m_pcrGraphCuvetteBoxes.append (ui->pcrGraphCuvette4Box);
  m_pcrGraphCuvetteBoxes.append (ui->pcrGraphCuvette5Box);
  m_pcrGraphCuvetteBoxes.append (ui->pcrGraphCuvette6Box);
  m_pcrGraphCuvetteBoxes.append (ui->pcrGraphCuvette7Box);

  ui->gripperComboBox->addItem ("Heater 65", QVariant (Pge::Heater65));
  ui->gripperComboBox->addItem ("Heater 95", QVariant (Pge::Heater95));
  ui->gripperComboBox->addItem ("Heatsealer", QVariant (Pge::HeatSealer));

  connect (&m_log, &Log::logReceived, this, &MainWindow::logReceived);
  connect (&m_controller, &Controller::stateChanged, this, &MainWindow::stateChanged);
  connect (&m_controller, &Controller::temperaturesReceived, this, &MainWindow::temperaturesReceived);
  connect (&m_controller, &Controller::temperaturesReceived, ui->temperatureWidget, &TemperaturePlot::temperaturesReceived);
  connect (&m_controller, &Controller::started, this, &MainWindow::started);
  connect (&m_controller, &Controller::progressed, this, &MainWindow::progressed);
  connect (&m_controller, &Controller::timeDataProcessed, ui->timeDataWidget, &TimeDataPlot::plot);
  connect (&m_controller, &Controller::spatialDataProcessed, ui->spatialDataWidget, &SpatialDataPlot::plot);
  connect (&m_controller, &Controller::cycleCompleted, ui->pcrGraphWidget, &PcrProfilePlot::plot);
  connect (&m_elapsedTimer, &SignalingElapsedTimer::elapsed, this, &MainWindow::elapsed);
  connect (&m_blinkTimer, &QTimer::timeout, this, &MainWindow::blinkTimeout);
  connect (ui->actionThermocyclerDiskConfiguration, &QAction::triggered, this, &MainWindow::actionThermocyclerDiskConfigurationTriggered);
  connect (ui->actionInductionSelect, &QAction::triggered, this, &MainWindow::actionInductionSelectTriggered);
  connect (ui->actionInductionConnect, &QAction::triggered, this, &MainWindow::actionInductionConnectTriggered);
  connect (ui->actionInductionDisconnect, &QAction::triggered, this, &MainWindow::actionInductionDisconnectTriggered);
  connect (ui->actionOpticsLedConfiguration, &QAction::triggered, this, &MainWindow::actionOpticsLedConfigurationTriggered);
  connect (ui->actionOpticsCuvetteConfiguration, &QAction::triggered, this, &MainWindow::actionOpticsCuvetteConfigurationTriggered);
  connect (ui->actionOpticsProcessingConfiguration, &QAction::triggered, this, &MainWindow::actionOpticsProcessingConfigurationTriggered);
  connect (ui->actionPcrProtocolOpen, &QAction::triggered, this, &MainWindow::actionPcrProtocolOpenTriggered);
  connect (ui->controlTemperatureStandby65Box, &QDoubleSpinBox::editingFinished, this, &MainWindow::controlTemperatureStandby65BoxEditingFinished);
  connect (ui->controlTemperatureStandby95Box, &QDoubleSpinBox::editingFinished, this, &MainWindow::controlTemperatureStandby95BoxEditingFinished);
  connect (ui->controlTemperatureStandbyHeatSealerBox, &QDoubleSpinBox::editingFinished, this, &MainWindow::controlTemperatureStandbyHeatSealerBoxEditingFinished);
  connect (ui->controlTemperatureOn65Box, &QCheckBox::clicked, this, &MainWindow::controlTemperatureStandby65BoxEditingFinished);
  connect (ui->controlTemperatureOn95Box, &QCheckBox::clicked, this, &MainWindow::controlTemperatureStandby95BoxEditingFinished);
  connect (ui->controlTemperatureOnHeatSealerBox, &QCheckBox::clicked, this, &MainWindow::controlTemperatureStandbyHeatSealerBoxEditingFinished);
  connect (ui->controlStorageDirButton, &QPushButton::clicked, this, &MainWindow::controlStorageDirButtonClicked);
  connect (ui->timeDataChannelPd0Box, &QCheckBox::clicked, this, &MainWindow::timeDataChannelBoxClicked);
  connect (ui->timeDataChannelPd1Box, &QCheckBox::clicked, this, &MainWindow::timeDataChannelBoxClicked);
  connect (ui->timeDataChannelPd2Box, &QCheckBox::clicked, this, &MainWindow::timeDataChannelBoxClicked);
  connect (ui->timeDataChannelPd3Box, &QCheckBox::clicked, this, &MainWindow::timeDataChannelBoxClicked);
  connect (ui->spatialDataChannelPd0Box, &QCheckBox::clicked, this, &MainWindow::spatialDataChannelBoxClicked);
  connect (ui->spatialDataChannelPd1Box, &QCheckBox::clicked, this, &MainWindow::spatialDataChannelBoxClicked);
  connect (ui->spatialDataChannelPd2Box, &QCheckBox::clicked, this, &MainWindow::spatialDataChannelBoxClicked);
  connect (ui->spatialDataChannelPd3Box, &QCheckBox::clicked, this, &MainWindow::spatialDataChannelBoxClicked);
  connect (ui->pcrGraphChannelPd0Box, &QCheckBox::clicked, this, &MainWindow::pcrGraphChannelBoxClicked);
  connect (ui->pcrGraphChannelPd1Box, &QCheckBox::clicked, this, &MainWindow::pcrGraphChannelBoxClicked);
  connect (ui->pcrGraphChannelPd2Box, &QCheckBox::clicked, this, &MainWindow::pcrGraphChannelBoxClicked);
  connect (ui->pcrGraphChannelPd3Box, &QCheckBox::clicked, this, &MainWindow::pcrGraphChannelBoxClicked);
  connect (ui->pcrGraphCuvette0Box, &QCheckBox::clicked, this, &MainWindow::pcrGraphCuvetteBoxClicked);
  connect (ui->pcrGraphCuvette1Box, &QCheckBox::clicked, this, &MainWindow::pcrGraphCuvetteBoxClicked);
  connect (ui->pcrGraphCuvette2Box, &QCheckBox::clicked, this, &MainWindow::pcrGraphCuvetteBoxClicked);
  connect (ui->pcrGraphCuvette3Box, &QCheckBox::clicked, this, &MainWindow::pcrGraphCuvetteBoxClicked);
  connect (ui->pcrGraphCuvette4Box, &QCheckBox::clicked, this, &MainWindow::pcrGraphCuvetteBoxClicked);
  connect (ui->pcrGraphCuvette5Box, &QCheckBox::clicked, this, &MainWindow::pcrGraphCuvetteBoxClicked);
  connect (ui->pcrGraphCuvette6Box, &QCheckBox::clicked, this, &MainWindow::pcrGraphCuvetteBoxClicked);
  connect (ui->pcrGraphCuvette7Box, &QCheckBox::clicked, this, &MainWindow::pcrGraphCuvetteBoxClicked);
  connect (ui->startButton, &QPushButton::clicked, this, &MainWindow::startButtonClicked);
  connect (ui->resumeButton, &QPushButton::clicked, this, &MainWindow::resumeButtonClicked);
  connect (ui->abortButton, &QPushButton::clicked, this, &MainWindow::abortButtonClicked);

  connect (ui->stepperHomeButton, &QPushButton::clicked, this, &MainWindow::stepperHomeButtonClicked);
  connect (ui->stepperRetractButton, &QPushButton::clicked, this, &MainWindow::stepperRetractButtonClicked);
  connect (ui->stepperMovePositionRelativeButton, &QPushButton::clicked, this, &MainWindow::stepperMovePositionRelativeButtonClicked);
  connect (ui->stepperMovePositionAbsoluteButton, &QPushButton::clicked, this, &MainWindow::stepperMovePositionAbsoluteButtonClicked);
  connect (ui->gripperReadButton, &QPushButton::clicked, this, &MainWindow::gripperReadButtonClicked);
  connect (ui->gripperWriteButton, &QPushButton::clicked, this, &MainWindow::gripperWriteButtonClicked);
  connect (ui->sealerOpenPushButton, &QPushButton::clicked, this, &MainWindow::sealerOpenPushButtonClicked);
  connect (ui->sealerClosePushButton, &QPushButton::clicked, this, &MainWindow::sealerClosePushButtonClicked);
  connect (ui->heater65OpenPushButton, &QPushButton::clicked, this, &MainWindow::heater65OpenPushButtonClicked);
  connect (ui->heater65ClosePushButton, &QPushButton::clicked, this, &MainWindow::heater65ClosePushButtonClicked);
  connect (ui->heater95OpenPushButton, &QPushButton::clicked, this, &MainWindow::heater95OpenPushButtonClicked);
  connect (ui->heater95ClosePushButton, &QPushButton::clicked, this, &MainWindow::heater95ClosePushButtonClicked);
  connect (ui->heaterBothOpenPushButton, &QPushButton::clicked, this, &MainWindow::heaterBothOpenPushButtonClicked);
  connect (ui->heaterBothClosePushButton, &QPushButton::clicked, this, &MainWindow::heaterBothClosePushButtonClicked);
  connect (ui->spindleSpinStartPushButton, &QPushButton::clicked, this, &MainWindow::spindleSpinStartPushButtonClicked);
  connect (ui->spindleStopPushButton, &QPushButton::clicked, this, &MainWindow::spindleStopPushButtonClicked);
  connect (ui->spindleDiskHomePushButton, &QPushButton::clicked, this, &MainWindow::spindleDiskHomePushButtonClicked);
  connect (ui->spindleMovePositionRelativePushButton, &QPushButton::clicked, this, &MainWindow::spindleMovePositionRelativePushButtonClicked);
  connect (ui->spindleMovePositionAbsolutePushButton, &QPushButton::clicked, this, &MainWindow::spindleMovePositionAbsolutePushButtonClicked);
  connect (ui->spindleMoveAngleRelativePushButton, &QPushButton::clicked, this, &MainWindow::spindleMoveAngleRelativePushButtonClicked);
  connect (ui->spindleMoveAngleAbsolutePushButton, &QPushButton::clicked, this, &MainWindow::spindleMoveAngleAbsolutePushButtonClicked);
  connect (ui->sealerSetpointDoubleSpinBox, &QDoubleSpinBox::editingFinished, this, &MainWindow::sealerSetpointDoubleSpinBoxEditingFinished);
  connect (ui->sealerCheckBox, &QCheckBox::clicked, this, &MainWindow::sealerSetpointDoubleSpinBoxEditingFinished);
  connect (ui->heater65SetpointDoubleSpinBox, &QDoubleSpinBox::editingFinished, this, &MainWindow::heater65SetpointDoubleSpinBoxEditingFinished);
  connect (ui->heater65CheckBox, &QCheckBox::clicked, this, &MainWindow::heater65SetpointDoubleSpinBoxEditingFinished);
  connect (ui->heater95SetpointDoubleSpinBox, &QDoubleSpinBox::editingFinished, this, &MainWindow::heater95SetpointDoubleSpinBoxEditingFinished);
  connect (ui->heater95CheckBox, &QCheckBox::clicked, this, &MainWindow::heater95SetpointDoubleSpinBoxEditingFinished);
  connect (ui->gripperComboBox, &QComboBox::activated, this, &MainWindow::gripperComboBoxActivated);
  connect (ui->gripperReadButton, &QPushButton::clicked, this, &MainWindow::gripperReadButtonClicked);
  connect (ui->gripperWriteButton, &QPushButton::clicked, this, &MainWindow::gripperWriteButtonClicked);
  connect (ui->amcUartDisablePushButton, &QPushButton::clicked, this, &MainWindow::amcUartDisablePushButtonClicked);
  connect (ui->led1CurrentDoubleSpinBox, &QDoubleSpinBox::editingFinished, this, &MainWindow::led1CurrentDoubleSpinBoxEditingFinished);
  connect (ui->led1ShuntDoubleSpinBox, &QDoubleSpinBox::editingFinished, this, &MainWindow::led1CurrentDoubleSpinBoxEditingFinished);
  connect (ui->led1CheckBox, &QCheckBox::clicked, this, &MainWindow::led1CurrentDoubleSpinBoxEditingFinished);
  connect (ui->led2CurrentDoubleSpinBox, &QDoubleSpinBox::editingFinished, this, &MainWindow::led2CurrentDoubleSpinBoxEditingFinished);
  connect (ui->led2ShuntDoubleSpinBox, &QDoubleSpinBox::editingFinished, this, &MainWindow::led2CurrentDoubleSpinBoxEditingFinished);
  connect (ui->led2CheckBox, &QCheckBox::clicked, this, &MainWindow::led2CurrentDoubleSpinBoxEditingFinished);
  connect (ui->led3CurrentDoubleSpinBox, &QDoubleSpinBox::editingFinished, this, &MainWindow::led3CurrentDoubleSpinBoxEditingFinished);
  connect (ui->led3ShuntDoubleSpinBox, &QDoubleSpinBox::editingFinished, this, &MainWindow::led3CurrentDoubleSpinBoxEditingFinished);
  connect (ui->led3CheckBox, &QCheckBox::clicked, this, &MainWindow::led3CurrentDoubleSpinBoxEditingFinished);
  connect (ui->led4CurrentDoubleSpinBox, &QDoubleSpinBox::editingFinished, this, &MainWindow::led4CurrentDoubleSpinBoxEditingFinished);
  connect (ui->led4ShuntDoubleSpinBox, &QDoubleSpinBox::editingFinished, this, &MainWindow::led4CurrentDoubleSpinBoxEditingFinished);
  connect (ui->led4CheckBox, &QCheckBox::clicked, this, &MainWindow::led4CurrentDoubleSpinBoxEditingFinished);

  configureUserInput ();
  restoreSettings ();

  ui->controlLed0Box->setChecked ((m_controller.ledEnable () & (1 << 0)) != 0);
  ui->controlLed1Box->setChecked ((m_controller.ledEnable () & (1 << 1)) != 0);
  ui->controlLed2Box->setChecked ((m_controller.ledEnable () & (1 << 2)) != 0);
  ui->controlLed3Box->setChecked ((m_controller.ledEnable () & (1 << 3)) != 0);

  ui->timeDataWidget->setChannelMask (maskFromUi (m_timeDataChannelBoxes));
  ui->spatialDataWidget->setChannelMask (maskFromUi (m_spatialDataChannelBoxes));
  ui->spatialDataWidget->setCuvetteConfig (m_controller.cuvetteConfig ());
  ui->pcrGraphWidget->setChannelMask (maskFromUi (m_pcrGraphChannelBoxes));
  ui->pcrGraphWidget->setCuvetteMask (maskFromUi (m_pcrGraphCuvetteBoxes));

  m_controller.openCycler ();
  m_controller.openOptics ();

  resetProgress ();
  initHeaters ();
  updateStatus ();

//  QStringList devices = Interface::listDevices ();
//  if (!devices.isEmpty ())
//  { if (m_interface.open (devices.first ()))
//      { ui->gripperBox->setEnabled (true);

//          ui->position1Box->setEnabled (true);
//          ui->force1Box->setEnabled (true);
//          ui->speed1Box->setEnabled (true);

//          ui->position2Box->setEnabled (true);
//          ui->force2Box->setEnabled (true);
//          ui->speed2Box->setEnabled (true);

//          ui->position3Box->setEnabled (true);
//          ui->force3Box->setEnabled (true);
//          ui->speed3Box->setEnabled (true);

//          ui->position4Box->setEnabled (true);
//          ui->force4Box->setEnabled (true);
//          ui->speed4Box->setEnabled (true);

//          ui->readButton->setEnabled (true);
//          ui->writeButton->setEnabled (true);

//          read ();
//      }
//  }
}

MainWindow::~MainWindow (void)
{ saveSettings ();
  m_controller.closeOptics ();
  m_controller.closeInduction ();
  m_controller.closeCycler ();
  delete ui;
}

void MainWindow::logReceived (const QDateTime &timestamp, const Log::Severity severity, const QString &source, const QString &text)
{ QString sev;

  switch (severity)
  { case Log::Debug: sev = "[**]"; break;
    case Log::Info: sev = "[II]"; break;
    case Log::Warning: sev = "[WW]"; break;
    case Log::Error: sev = "[EE]"; break;
    default: sev = "[??]";
  }

  QTextStream (stdout) << timestamp.toString ("yyyy-MM-ddThh:mm:ss.zzz") << " " << sev << " " << source << ": " << text << Qt::endl;
}

void MainWindow::stateChanged (const Controller::State newState)
{ updateStatus ();
  setEnabled ((newState == Controller::Running) || (newState == Controller::PcrPaused) || (newState == Controller::PreHbhPaused));
  printf("stateChanged = %d\n", (int)newState);

  ui->startButton->setEnabled (newState == Controller::Ready);
  ui->resumeButton->setEnabled ((newState == Controller::PcrPaused) || (newState == Controller::PreHbhPaused));
  ui->abortButton->setEnabled ((newState == Controller::Running) || (newState == Controller::PcrPaused) || (newState == Controller::PreHbhPaused));

  if ((newState == Controller::NotReady) || (newState == Controller::Ready))
  { m_elapsedTimer.stop ();
    m_blinkTimer.stop ();
    resetProgress ();
  }
}

void MainWindow::temperaturesReceived (const QList<double> &top65, const QList<double> &bottom65, const QList<double> &top95, const QList<double> &bottom95, const double heatSealer)
{ updateTemperature (ui->controlTemperatureActual65TopEdit, top65);
  updateTemperature (ui->controlTemperatureActual65BottomEdit, bottom65);
  updateTemperature (ui->controlTemperatureActual95TopEdit, top95);
  updateTemperature (ui->controlTemperatureActual95BottomEdit, bottom95);
  ui->controlTemperatureActualHeatSealerEdit->setText (QString::number (heatSealer, 'f', 1) + " °C");

  updateTemperature (ui->heater65TopActualLineEdit, top65);
  updateTemperature (ui->heater65BottomActualLineEdit, bottom65);
  updateTemperature (ui->heater95TopActualLineEdit, top95);
  updateTemperature (ui->heater95BottomActualLineEdit, bottom95);
  ui->sealerTempActualLineEdit->setText (QString::number (heatSealer, 'f', 1) + " °C");
}

void MainWindow::started (const int steps)
{ ui->controlProgressBar->setMaximum (steps);
}

void MainWindow::progressed (const int step, const Progress &progress)
{ m_log.debug ("MainWindow", "progressed: step=" + QString::number (step) + ", state=" + QString::number (progress.state ()) + ", subState=" + QString::number (progress.subState ()) + ", index=" + QString::number (progress.index ()));

  ui->controlProgressBar->setValue (step);

  if (m_progress != progress.state ())
  { m_progress = progress.state ();
    m_blink = true;
    m_blinkTimer.start (500);
    //printf("MainWindow, m_blink=true, \n");
    updateProgress ();
  }

  if (m_progress == Progress::Thermocycling) ui->controlProgressThermocyclingButton->setText ("Thermocycling (" + (progress.index () > 0?"cycle " + QString::number (progress.index ()):"waiting for heaters") + ")");
}

void MainWindow::elapsed (const qint64 time)
{ int sec10 = time % 10;
  int sec = time / 10;
  int min = sec / 60;

  ui->controlProgressElapsedTimeEdit->setText (QString ("%1:%2.%3").arg (min).arg (sec % 60, 2, 10, QChar ('0')).arg (sec10));
}

void MainWindow::blinkTimeout (void)
{ m_blink = !m_blink;
  updateProgress ();
}

void MainWindow::actionThermocyclerDiskConfigurationTriggered (void)
{ DiskConfigurationDialog dialog (m_controller.diskConfig (), this);
  if (dialog.exec () == DiskConfigurationDialog::Accepted)
  { m_controller.setDiskConfig (dialog.configuration ());
  }
}

void MainWindow::actionInductionSelectTriggered (void)
{ QString portName = SerialPortSelectionDialog::getPort (this).portName ();
  if (!portName.isEmpty ())
  { if (m_controller.openInduction (portName))
    { m_portNameInduction = portName;
      updateStatus ();
    } else QMessageBox::critical (this, "Error", "Cannot connect to induction.");
  }
}

void MainWindow::actionInductionConnectTriggered (void)
{ if (m_controller.openInduction (m_portNameInduction))
  { updateStatus ();
  } else QMessageBox::critical (this, "Error", "Cannot connect to induction.");
}

void MainWindow::actionInductionDisconnectTriggered (void)
{ m_controller.closeInduction ();
  updateStatus ();
}

void MainWindow::actionOpticsLedConfigurationTriggered (void)
{ LedConfigurationDialog dialog (m_controller.ledConfig (), this);
  if (dialog.exec () == LedConfigurationDialog::Accepted) m_controller.setLedConfig (dialog.configuration ());
}

void MainWindow::actionOpticsCuvetteConfigurationTriggered (void)
{ CuvetteLocationConfigurationDialog dialog (m_controller.cuvetteConfig (), this);
  if (dialog.exec () == CuvetteLocationConfigurationDialog::Accepted)
  { m_controller.setCuvetteConfig (dialog.configuration ());
    ui->spatialDataWidget->setCuvetteConfig (dialog.configuration ());
  }
}

void MainWindow::actionOpticsProcessingConfigurationTriggered (void)
{ CuvetteProcessingConfigurationDialog dialog (m_controller.processingConfig (), this);
  if (dialog.exec () == CuvetteProcessingConfigurationDialog::Accepted) m_controller.setProcessingConfig (dialog.configuration ());
}

void MainWindow::actionPcrProtocolOpenTriggered (void)
{ QString fileName = QFileDialog::getOpenFileName (this, "Open PCR protocol", QString (), "PCR protocol files (*.pcrprotocol)");
  if (!fileName.isEmpty ()) loadProtocol (fileName);
}

void MainWindow::actionPcrProtocolRecentlyUsedTriggered (void)
{ loadProtocol (static_cast<QAction *> (sender ())->text ());
}

void MainWindow::controlTemperatureStandby65BoxEditingFinished (void)
{ if (ui->controlTemperatureOn65Box->isChecked ()) m_controller.standby65On (ui->controlTemperatureStandby65Box->value ()); else m_controller.standby65Off ();
}

void MainWindow::controlTemperatureStandby95BoxEditingFinished (void)
{ if (ui->controlTemperatureOn95Box->isChecked ()) m_controller.standby95On (ui->controlTemperatureStandby95Box->value ()); else m_controller.standby95Off ();
}

void MainWindow::controlTemperatureStandbyHeatSealerBoxEditingFinished (void)
{ if (ui->controlTemperatureOnHeatSealerBox->isChecked ()) m_controller.standbySealerOn (ui->controlTemperatureStandbyHeatSealerBox->value ()); else m_controller.standbySealerOff ();
}

void MainWindow::controlStorageDirButtonClicked (void)
{ QString directory = QFileDialog::getExistingDirectory (this, "Select directory", ui->controlStorageDirEdit->text ());
  if (!directory.isEmpty ()) ui->controlStorageDirEdit->setText (directory);
}

void MainWindow::timeDataChannelBoxClicked (void)
{ ui->timeDataWidget->setChannelMask (maskFromUi (m_timeDataChannelBoxes));
}

void MainWindow::spatialDataChannelBoxClicked (void)
{ ui->spatialDataWidget->setChannelMask (maskFromUi (m_spatialDataChannelBoxes));
}

void MainWindow::pcrGraphChannelBoxClicked (void)
{ ui->pcrGraphWidget->setChannelMask (maskFromUi (m_pcrGraphChannelBoxes));
}

void MainWindow::pcrGraphCuvetteBoxClicked (void)
{ ui->pcrGraphWidget->setCuvetteMask (maskFromUi (m_pcrGraphCuvetteBoxes));
}

void MainWindow::startButtonClicked (void)
{ ui->startButton->setEnabled (false);

  int ledMask = 0;
  if (ui->controlLed0Box->isChecked ()) ledMask |= (1 << 0);
  if (ui->controlLed1Box->isChecked ()) ledMask |= (1 << 1);
  if (ui->controlLed2Box->isChecked ()) ledMask |= (1 << 2);
  if (ui->controlLed3Box->isChecked ()) ledMask |= (1 << 3);
  m_controller.setLedEnable (ledMask);

  // Ming: 10/16/23: for selectable cuvette sealer
  int sealerMask = 0;
  if (ui->sealerPosition0CheckBox->isChecked ()) sealerMask |= (1 << 0);
  if (ui->sealerPosition1CheckBox->isChecked ()) sealerMask |= (1 << 1);
  if (ui->sealerPosition2CheckBox->isChecked ()) sealerMask |= (1 << 2);
  if (ui->sealerPosition3CheckBox->isChecked ()) sealerMask |= (1 << 3);
  if (ui->sealerPosition4CheckBox->isChecked ()) sealerMask |= (1 << 4);
  if (ui->sealerPosition5CheckBox->isChecked ()) sealerMask |= (1 << 5);
  if (ui->sealerPosition6CheckBox->isChecked ()) sealerMask |= (1 << 6);
  if (ui->sealerPosition7CheckBox->isChecked ()) sealerMask |= (1 << 7);
  if (ui->sealerPosition8CheckBox->isChecked ()) sealerMask |= (1 << 8);
  m_controller.setSealerEnable (sealerMask);

  if (m_controller.start (ui->controlStorageDirEdit->text (), ui->controlStorageFileNameEdit->text (), ui->runPreHbhBox->isChecked (), ui->pcrPauseBox->isChecked (), ui->preHbhPauseBox->isChecked (), UserInputWidgets::data (m_userInputConfig, m_userInputWidgets)))
  { ui->controlProgressPreHbhButton->setEnabled (ui->runPreHbhBox->isChecked());
    ui->controlProgressPreHbhPausedButton->setEnabled (ui->preHbhPauseBox->isChecked () && ui->runPreHbhBox->isChecked());
    ui->controlProgressPcrPausedButton->setEnabled (ui->pcrPauseBox->isChecked ());
    m_elapsedTimer.start (100);
    m_progress = Progress::Unknown;
  } else ui->startButton->setEnabled (true);
}

void MainWindow::resumeButtonClicked (void)
{ m_controller.resume ();
}

void MainWindow::abortButtonClicked (void)
{ m_controller.abort ();
}

void MainWindow::configureUserInput (void)
{ QFile file ("user_input.json");
  if (file.open (QFile::ReadOnly))
  { QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson (file.readAll (), &error);
    file.close ();

    if (error.error == QJsonParseError::NoError)
    { m_userInputConfig = UserInputConfiguration (doc.array ());
    }
  }

  m_userInputWidgets = UserInputWidgets::widgets (m_userInputConfig, UserInputData (m_userInputConfig), ui->controlUserInputLayout, this);
}

void MainWindow::loadProtocol (const QString &fileName)
{ QFile file (fileName);
  if (file.open (QFile::ReadOnly))
  { QByteArray data = file.readAll ();
    file.close ();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson (data, &error);
    if (error.error == QJsonParseError::NoError)
    { PcrProtocol protocol (doc.object ());
      m_controller.setProtocol (protocol);

      updateStatus ();

      ui->controlProgressSamplePrepButton->setEnabled (protocol.samplePrepConfig ().enable ());
      ui->controlProgressMeteringButton->setEnabled (protocol.samplePrepConfig ().enable ());
      ui->controlProgressResuspensionButton->setEnabled (protocol.samplePrepConfig ().enable ());

      setWindowTitle ("FastPCRController2 - " + fileName);

      updateRecentlyUsedProtocols (fileName);
    } else QMessageBox::critical (this, "Error", "JSON parse error.");
  } else QMessageBox::critical (this, "Error", "Cannot open file '" + fileName + "'.");
}

void MainWindow::updateRecentlyUsedProtocols (const QString &fileName)
{ if (!fileName.isEmpty () && QFile::exists (fileName))
  { m_recentlyUsedProtocols.prepend (fileName);
    for (int index = 1; index < m_recentlyUsedProtocols.count ();) if (m_recentlyUsedProtocols.at (index) == fileName) m_recentlyUsedProtocols.removeAt (index); else index++;
    while (m_recentlyUsedProtocols.count () > 3) m_recentlyUsedProtocols.removeLast ();

    ui->menuPcrProtocolRecentlyUsed->clear ();
    for (const QString &str : m_recentlyUsedProtocols) ui->menuPcrProtocolRecentlyUsed->addAction (str);

    for (const QAction *action : ui->menuPcrProtocolRecentlyUsed->actions ()) connect (action, &QAction::triggered, this, &MainWindow::actionPcrProtocolRecentlyUsedTriggered);
  }
}

void MainWindow::setEnabled (const bool running)
{ if (running)
  { ui->actionThermocyclerDiskConfiguration->setEnabled (false);

    ui->actionInductionSelect->setEnabled (false);
    ui->actionInductionConnect->setEnabled (false);
    ui->actionInductionDisconnect->setEnabled (false);

    ui->actionOpticsLedConfiguration->setEnabled (false);
    ui->actionOpticsCuvetteConfiguration->setEnabled (false);
    ui->actionOpticsProcessingConfiguration->setEnabled (false);

    ui->actionPcrProtocolOpen->setEnabled (false);
    ui->menuPcrProtocolRecentlyUsed->setEnabled (false);

    ui->controlStorageDirButton->setEnabled (false);
    ui->controlStorageFileNameEdit->setEnabled (false);

    ui->controlUserInputBox->setEnabled (false);

    ui->runPreHbhBox->setEnabled (false);
    ui->pcrPauseBox->setEnabled (false);
    ui->preHbhPauseBox->setEnabled (false);
  } else
  { ui->actionThermocyclerDiskConfiguration->setEnabled (true);

    ui->actionInductionSelect->setEnabled (true);
    ui->actionInductionConnect->setEnabled (!m_portNameInduction.isEmpty () && !m_controller.isOpenInduction ());
    ui->actionInductionDisconnect->setEnabled (m_controller.isOpenInduction ());

    ui->actionOpticsLedConfiguration->setEnabled (true);
    ui->actionOpticsCuvetteConfiguration->setEnabled (true);
    ui->actionOpticsProcessingConfiguration->setEnabled (true);

    ui->actionPcrProtocolOpen->setEnabled (true);

    ui->controlStorageDirButton->setEnabled (true);
    ui->controlStorageFileNameEdit->setEnabled (true);

    ui->controlUserInputBox->setEnabled (true);

    ui->runPreHbhBox->setEnabled (true);
    ui->pcrPauseBox->setEnabled (true);
    ui->preHbhPauseBox->setEnabled (true);
  }
}

void MainWindow::resetProgress (void)
{ ui->controlProgressPreHbhButton->setChecked (false);
  ui->controlProgressPreHbhPausedButton->setChecked (false);
  ui->controlProgressPreTestButton->setChecked (false);
  ui->controlProgressSamplePrepButton->setChecked (false);
  ui->controlProgressMeteringButton->setChecked (false);
  ui->controlProgressResuspensionButton->setChecked (false);
  ui->controlProgressFillingButton->setChecked (false);
  ui->controlProgressPcrPausedButton->setChecked (false);
  ui->controlProgressRtButton->setChecked (false);
  ui->controlProgressThermocyclingButton->setChecked (false);
  ui->controlProgressPostTestButton->setChecked (false);

  ui->controlProgressThermocyclingButton->setText ("Thermocycling");
}

void MainWindow::updateProgress (void)
{ //printf("MainWindow:updateProgress, m_progress= %d\n", (int)m_progress);
  ui->controlProgressPreHbhButton->setChecked (m_blink && (m_progress == Progress::PreHbhSpinAndMixing));
  ui->controlProgressPreHbhPausedButton->setChecked (m_blink && (m_progress == Progress::PreHbhPausing));
  ui->controlProgressPreTestButton->setChecked (m_blink && (m_progress == Progress::PreTest));
  ui->controlProgressSamplePrepButton->setChecked (m_blink && (m_progress == Progress::SamplePrep));
  ui->controlProgressMeteringButton->setChecked (m_blink && (m_progress == Progress::Metering));
  ui->controlProgressResuspensionButton->setChecked (m_blink && (m_progress == Progress::Resuspension));
  ui->controlProgressFillingButton->setChecked (m_blink && (m_progress == Progress::Filling));
  ui->controlProgressPcrPausedButton->setChecked (m_blink && (m_progress == Progress::PcrPausing));
  ui->controlProgressRtButton->setChecked (m_blink && (m_progress == Progress::Rt));
  ui->controlProgressThermocyclingButton->setChecked (m_blink && (m_progress == Progress::Thermocycling));
  ui->controlProgressPostTestButton->setChecked (m_blink && (m_progress == Progress::PostTest));
}

void MainWindow::initHeaters (void)
{ if (ui->controlTemperatureOn65Box->isChecked ()) m_controller.standby65On (ui->controlTemperatureStandby65Box->value ()); else m_controller.standby65Off ();
  if (ui->controlTemperatureOn95Box->isChecked ()) m_controller.standby95On (ui->controlTemperatureStandby95Box->value ()); else m_controller.standby95Off ();
  if (ui->controlTemperatureOnHeatSealerBox->isChecked ()) m_controller.standbySealerOn (ui->controlTemperatureStandbyHeatSealerBox->value ()); else m_controller.standbySealerOff ();
}

void MainWindow::updateStatus (void)
{ updateStatus (ui->controlStatusCyclerEdit, m_controller.isOpenCycler (), "Connected", "Not connected");
  updateStatus (ui->controlStatusInductionEdit, m_controller.isOpenInduction (), "Connected", "Not connected");
  updateStatus (ui->controlStatusOpticsEdit, m_controller.isOpenOptics (), "Connected", "Not connected");
  updateStatus (ui->controlStatusProtocolEdit, m_controller.hasProtocol (), "Loaded", "Not loaded");
}

void MainWindow::restoreSettings (void)
{ QJsonObject object = Settings::restore ("FastPCRController2").object ();

  m_controller.setDiskConfig (DiskConfiguration (object.value ("diskConfiguration").toObject ()));
  m_controller.setLedConfig (LedConfigurationContainer (object.value ("ledConfiguration").toArray ()));
  m_controller.setLedEnable (object.value ("ledEnable").toInt ());
  m_controller.setCuvetteConfig (CuvetteLocationConfigurationContainer (object.value ("cuvetteConfiguration").toArray ()));
  m_controller.setProcessingConfig (CuvetteProcessingConfiguration (object.value ("processingConfiguration").toObject ()));
  maskToUi (object.value ("pcrGraphChannelMask").toInt (15), m_pcrGraphChannelBoxes);
  maskToUi (object.value ("pcrGraphCuvetteMask").toInt (255), m_pcrGraphCuvetteBoxes);
  maskToUi (object.value ("timeDataChannelMask").toInt (15), m_timeDataChannelBoxes);
  maskToUi (object.value ("spatialDataChannelMask").toInt (15), m_spatialDataChannelBoxes);
  heatersFromJson (object.value ("heaters").toObject ());
}

void MainWindow::saveSettings (void)
{ QJsonObject object;

  object.insert ("diskConfiguration", m_controller.diskConfig ().toJson ());
  object.insert ("ledConfiguration", m_controller.ledConfig ().toJson ());
  object.insert ("ledEnable", m_controller.ledEnable ());
  object.insert ("cuvetteConfiguration", m_controller.cuvetteConfig ().toJson ());
  object.insert ("processingConfiguration", m_controller.processingConfig ().toJson ());
  object.insert ("pcrGraphChannelMask", maskFromUi (m_pcrGraphChannelBoxes));
  object.insert ("pcrGraphCuvetteMask", maskFromUi (m_pcrGraphCuvetteBoxes));
  object.insert ("timeDataChannelMask", maskFromUi (m_timeDataChannelBoxes));
  object.insert ("spatialDataChannelMask", maskFromUi (m_spatialDataChannelBoxes));
  object.insert ("heaters", heatersToJson ());

  Settings::save ("FastPCRController2", QJsonDocument (object));
}

void MainWindow::heatersFromJson (const QJsonObject &object)
{ ui->controlTemperatureStandby65Box->setValue (object.value ("standby65").toDouble (50.0));
  ui->controlTemperatureStandby95Box->setValue (object.value ("standby95").toDouble (50.0));
  ui->controlTemperatureStandbyHeatSealerBox->setValue (object.value ("standbyHeatSealer").toDouble (50.0));
  ui->controlTemperatureOn65Box->setChecked (object.value ("on65").toBool (false));
  ui->controlTemperatureOn95Box->setChecked (object.value ("on95").toBool (false));
  ui->controlTemperatureOnHeatSealerBox->setChecked (object.value ("onheatSealer").toBool (false));
}

QJsonObject MainWindow::heatersToJson (void) const
{ QJsonObject object;
  object.insert ("standby65", ui->controlTemperatureStandby65Box->value ());
  object.insert ("standby95", ui->controlTemperatureStandby95Box->value ());
  object.insert ("standbyHeatSealer", ui->controlTemperatureStandbyHeatSealerBox->value ());
  object.insert ("on65", ui->controlTemperatureOn65Box->isChecked ());
  object.insert ("on95", ui->controlTemperatureOn95Box->isChecked ());
  object.insert ("onHeatSealer", ui->controlTemperatureOnHeatSealerBox->isChecked ());
  return object;
}

void MainWindow::updateStatus (QLineEdit *edit, const bool positive, const QString &positiveString, const QString &negativeString)
{ edit->setText (positive?positiveString:negativeString);

  QPalette pal;
  pal.setColor (QPalette::Base, (positive?Qt::green:Qt::red));
  edit->setPalette (pal);
}

void MainWindow::updateTemperature (QLineEdit *edit, const QVector<double> &temperatures)
{ edit->setText (QString::number ((temperatures.at (0) + temperatures.at (1)) / 2.0, 'f', 1) + " °C");
}

int MainWindow::maskFromUi (const QList<QCheckBox *> &boxes)
{ int result = 0;
  for (int bit = 0; bit < boxes.count (); bit++) if (boxes.at (bit)->isChecked ()) result |= (1 << bit);
  return result;
}

void MainWindow::maskToUi (const int mask, const QList<QCheckBox *> &boxes)
{ for (int bit = 0; bit < boxes.count (); bit++) boxes.at (bit)->setChecked ((mask & (1 << bit)) != 0);
}

void MainWindow::stepperRetractButtonClicked(void)
{
  ui->stepperRetractButton->setEnabled (false);
  m_controller.stepperRetract ();
  ui->stepperRetractButton->setEnabled (true);
}

void MainWindow::stepperHomeButtonClicked(void)
{
  ui->stepperHomeButton->setEnabled (false);
  m_controller.stepperHome ();
  ui->stepperHomeButton->setEnabled (true);
}

void MainWindow::stepperMovePositionRelativeButtonClicked(void)
{
  QString position = ui->stepperPositionLineEdit->text();
  int iPosition = position.toInt();
  QString maxSpeed = ui->stepperMaxSpeedLineEdit->text();
  double dMaxSpeed = maxSpeed.toDouble();

  ui->stepperMovePositionRelativeButton->setEnabled (false);
  m_controller.stepperMovePositionRelative (iPosition, dMaxSpeed);
  ui->stepperMovePositionRelativeButton->setEnabled (true);
}

void MainWindow::stepperMovePositionAbsoluteButtonClicked(void)
{
  QString position = ui->stepperPositionLineEdit->text();
  int iPosition = position.toInt();
  QString maxSpeed = ui->stepperMaxSpeedLineEdit->text();
  double dMaxSpeed = maxSpeed.toDouble();

  ui->stepperMovePositionAbsoluteButton->setEnabled (false);
  m_controller.stepperMovePositionAbsolute (iPosition, dMaxSpeed);
  ui->stepperMovePositionAbsoluteButton->setEnabled (true);
}


void MainWindow::sealerClosePushButtonClicked(void)
{
  ui->sealerClosePushButton->setEnabled (false);
  m_controller.setSealerClose();
  ui->sealerClosePushButton->setEnabled (true);
}

void MainWindow::sealerOpenPushButtonClicked(void)
{
  ui->sealerOpenPushButton->setEnabled (false);
  m_controller.setSealerOpen();
  ui->sealerOpenPushButton->setEnabled (true);
}

void MainWindow::heater65ClosePushButtonClicked(void)
{
  ui->heater65ClosePushButton->setEnabled (false);
  m_controller.setHeater65Close();
  ui->heater65ClosePushButton->setEnabled (true);
}

void MainWindow::heater65OpenPushButtonClicked(void)
{
  ui->heater65OpenPushButton->setEnabled (false);
  m_controller.setHeater65Open();
  ui->heater65OpenPushButton->setEnabled (true);
}

void MainWindow::heater95ClosePushButtonClicked(void)
{
  ui->heater95ClosePushButton->setEnabled (false);
  m_controller.setHeater95Close();
  ui->heater95ClosePushButton->setEnabled (true);
}

void MainWindow::heater95OpenPushButtonClicked(void)
{
  ui->heater95OpenPushButton->setEnabled (false);
  m_controller.setHeater95Open();
  ui->heater95OpenPushButton->setEnabled (true);
}

void MainWindow::heaterBothClosePushButtonClicked(void)
{
  ui->heaterBothClosePushButton->setEnabled (false);
  m_controller.setHeaterBothClose();
  ui->heaterBothClosePushButton->setEnabled (true);
}

void MainWindow::heaterBothOpenPushButtonClicked(void)
{
  ui->heaterBothOpenPushButton->setEnabled (false);
  m_controller.setHeaterBothOpen();
  ui->heaterBothOpenPushButton->setEnabled (true);
}

void MainWindow::spindleSpinStartPushButtonClicked(void)
{
  QString velocity = ui->velocityLineEdit->text();
  int iVelocity = velocity.toInt();
  QString accel = ui->accelLineEdit->text();
  int iAccel = accel.toInt();
  QString decel = ui->decelLineEdit->text();
  int iDecel = decel.toInt();

  ui->spindleSpinStartPushButton->setEnabled (false);
  m_controller.spindleSpinStart(iVelocity, iAccel, iDecel);
  ui->spindleSpinStartPushButton->setEnabled (true);
}

void MainWindow::spindleStopPushButtonClicked(void)
{
  ui->spindleStopPushButton->setEnabled (false);
  m_controller.spindleStop();
  ui->spindleStopPushButton->setEnabled (true);
}

void MainWindow::spindleDiskHomePushButtonClicked(void)
{
  ui->spindleDiskHomePushButton->setEnabled (false);
  if (m_controller.spindleDiskHome(Interface::Disk))
  {
    ui->spindleDiskHomePushButton->setEnabled (true);
  }
}

void MainWindow::spindleMovePositionRelativePushButtonClicked(void)
{

}

void MainWindow::spindleMovePositionAbsolutePushButtonClicked(void)
{

}

void MainWindow::spindleMoveAngleRelativePushButtonClicked(void)
{

}

void MainWindow::spindleMoveAngleAbsolutePushButtonClicked(void)
{

}

void MainWindow::sealerSetpointDoubleSpinBoxEditingFinished()
{
  if (ui->sealerCheckBox->isChecked ())
    m_controller.setSealerOn (ui->sealerSetpointDoubleSpinBox->value ());
  else
    m_controller.setSealerOff ();
}

void MainWindow::heater65SetpointDoubleSpinBoxEditingFinished()
{
  if (ui->heater65CheckBox->isChecked ())
    m_controller.setHeater65On (ui->heater65SetpointDoubleSpinBox->value ());
  else
    m_controller.setHeater65Off ();
}

void MainWindow::heater95SetpointDoubleSpinBoxEditingFinished()
{
  if (ui->heater95CheckBox->isChecked ())
    m_controller.setHeater95On (ui->heater95SetpointDoubleSpinBox->value ());
  else
    m_controller.setHeater95Off ();
}

void MainWindow::gripperComboBoxActivated(int index)
{

}

void MainWindow::gripperReadButtonClicked(void)
{

}

void MainWindow::gripperWriteButtonClicked(void)
{

}

void MainWindow::amcUartDisablePushButtonClicked(void)
{ printf("AMC UART Disable clicked");
  m_controller.amcUartDisable();
}

void MainWindow::led1CurrentDoubleSpinBoxEditingFinished(void)
{ // Ming: 10/19/23, LED1 on/off feature for engineering tab
  if (ui->led1CheckBox->isChecked ())
    m_controller.ledOnEngineering (0, ui->led1CurrentDoubleSpinBox->value (), ui->led1ShuntDoubleSpinBox->value ());
  else
    m_controller.ledOffEngineering (0);
}

void MainWindow::led2CurrentDoubleSpinBoxEditingFinished(void)
{ // Ming: 10/19/23, LED2 on/off feature for engineering tab
  if (ui->led2CheckBox->isChecked ())
    m_controller.ledOnEngineering (1, ui->led2CurrentDoubleSpinBox->value (), ui->led2ShuntDoubleSpinBox->value ());
  else
    m_controller.ledOffEngineering (1);
}

void MainWindow::led3CurrentDoubleSpinBoxEditingFinished(void)
{ // Ming: 10/19/23, LED3 on/off feature for engineering tab
  if (ui->led3CheckBox->isChecked ())
    m_controller.ledOnEngineering (2, ui->led3CurrentDoubleSpinBox->value (), ui->led3ShuntDoubleSpinBox->value ());
  else
    m_controller.ledOffEngineering (2);
}

void MainWindow::led4CurrentDoubleSpinBoxEditingFinished(void)
{ // Ming: 10/19/23, LED4 on/off feature for engineering tab
  if (ui->led4CheckBox->isChecked ())
    m_controller.ledOnEngineering (3, ui->led4CurrentDoubleSpinBox->value (), ui->led4ShuntDoubleSpinBox->value ());
  else
    m_controller.ledOffEngineering (3);
}

