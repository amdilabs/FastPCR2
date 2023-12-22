#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

const double MainWindow::decelerationInterval = 0.5;

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent), ui (new Ui::MainWindow)
{ ui->setupUi (this);

  connect (ui->actionOpen, &QAction::triggered, this, &MainWindow::actionOpenTriggered);
  connect (ui->actionSave, &QAction::triggered, this, &MainWindow::actionSaveTriggered);
  connect (ui->actionSaveAs, &QAction::triggered, this, &MainWindow::actionSaveAsTriggered);

  populate (PcrProtocol ());
}

MainWindow::~MainWindow (void)
{ delete ui;
}

void MainWindow::actionOpenTriggered (void)
{ QString fileName = QFileDialog::getOpenFileName (this, "Open", m_fileName, "PCR protocol files (*.pcrprotocol)");
  if (!fileName.isEmpty ())
  { QFile file (fileName);
    if (file.open (QFile::ReadOnly))
    { QJsonParseError error;
      QJsonDocument doc = QJsonDocument::fromJson (file.readAll (), &error);
      file.close ();

      if (error.error == QJsonParseError::NoError)
      { populate (PcrProtocol (doc.object ()));
        m_fileName = fileName;

        setWindowTitle ("FastPCRConfig - " + fileName);
      } else QMessageBox::critical (this, "Error", "JSON parse error (" + error.errorString () + ").");
    } else QMessageBox::critical (this, "Error", "Cannot open file '" + fileName + "'.");
  }
}

void MainWindow::actionSaveTriggered (void)
{ if (!m_fileName.isEmpty())
  { if (!save (m_fileName)) QMessageBox::critical (this, "Error", "Cannot save file '" + m_fileName + "'.");
  } else actionSaveAsTriggered ();
}

void MainWindow::actionSaveAsTriggered (void)
{ QString fileName = QFileDialog::getSaveFileName (this, "Save as", m_fileName, "PCR protocol files (*.pcrprotocol)");
  if (!fileName.isEmpty ())
  { if (save (fileName))
    { m_fileName = fileName;
    } else QMessageBox::critical (this, "Error", "Cannot save file '" + fileName + "'.");
  }
}

void MainWindow::populate (const PcrProtocol &protocol)
{ PreHbhConfiguration preHbhConfig = protocol.preHbhConfig ();
  ui->preHbhVelocityBox->setValue (qRound (preHbhConfig.velocity ()));
  ui->preHbhTimeBox->setValue (preHbhConfig.time ());
  ui->preHbhAccelerationBox->setValue (qRound (preHbhConfig.acceleration ()));
  ui->preHbhDecelerationBox->setValue (qRound (preHbhConfig.deceleration ()));
  ui->preHbhMixingNumOfCyclesBox->setValue (preHbhConfig.mixing ().numOfCycles ());
  ui->preHbhMixingIntervalBox->setValue (preHbhConfig.mixing ().interval ());
  ui->preHbhMixingVelocityBox->setValue (qRound (preHbhConfig.mixing ().velocity ()));
  ui->preHbhMixingAccelerationBox->setValue (qRound (preHbhConfig.mixing ().acceleration ()));
  ui->preHbhMixingDecelerationBox->setValue (qRound (preHbhConfig.mixing ().deceleration ()));

  SamplePrepConfiguration samplePrepConfig = protocol.samplePrepConfig ();
  ui->samplePrepEnableBox->setChecked (samplePrepConfig.enable ());
  ui->samplePrepPwmFreqBox->setValue (static_cast<double> (samplePrepConfig.pwmFreq ()) / 1000.0);
  ui->samplePrepHeatingDurationBox->setValue (samplePrepConfig.heatingDuration ());

  MeteringConfiguration meteringConfig = protocol.meteringConfig ();
  ui->meteringEvacuationVelocityBox->setValue (qRound (meteringConfig.evacuationVelocity ()));
  ui->meteringEvacuationTimeBox->setValue (meteringConfig.evacuationTime ());
  ui->meteringMeteringVelocityBox->setValue (qRound (meteringConfig.meteringVelocity ()));
  ui->meteringMeteringTimeBox->setValue (meteringConfig.meteringTime ());
  ui->meteringAccelerationBox->setValue (qRound (meteringConfig.acceleration ()));
  ui->meteringDecelerationBox->setValue (qRound (meteringConfig.deceleration ()));

  ResuspensionConfiguration resuspensionConfig = protocol.resuspensionConfig ();
  ui->resuspensionVelocityBox->setValue (qRound (resuspensionConfig.velocity ()));
  ui->resuspensionTimeBox->setValue (resuspensionConfig.time ());
  ui->resuspensionAccelerationBox->setValue (qRound (resuspensionConfig.acceleration ()));
  ui->resuspensionDecelerationBox->setValue (qRound (resuspensionConfig.deceleration ()));
  ui->resuspensionMixingNumOfCyclesBox->setValue (resuspensionConfig.mixing ().numOfCycles ());
  ui->resuspensionMixingIntervalBox->setValue (resuspensionConfig.mixing ().interval ());
  ui->resuspensionMixingVelocityBox->setValue (qRound (resuspensionConfig.mixing ().velocity ()));
  ui->resuspensionMixingAccelerationBox->setValue (qRound (resuspensionConfig.mixing ().acceleration ()));
  ui->resuspensionMixingDecelerationBox->setValue (qRound (resuspensionConfig.mixing ().deceleration ()));

  FillingConfiguration fillingConfig = protocol.fillingConfig ();
  ui->fillingVelocityBox->setValue (qRound (fillingConfig.velocity ()));
  ui->fillingTimeBox->setValue (fillingConfig.time ());
  ui->fillingAccelerationBox->setValue (qRound (fillingConfig.acceleration ()));
  ui->fillingDecelerationBox->setValue (qRound (fillingConfig.deceleration ()));
  ui->fillingSealingTemperatureBox->setValue (fillingConfig.sealerTemperature ());
  ui->fillingSealingTimeBox->setValue (fillingConfig.sealingTime ());
  ui->fillingSealingReverseBox->setChecked (fillingConfig.reverse ());

  RtConfiguration rtConfig = protocol.rtConfig ();
  ui->rtRtEnableBox->setChecked (rtConfig.rtEnable ());
  ui->rtRtTemperatureBox->setValue (rtConfig.rtTemperature ());
  ui->rtRtTimeBox->setValue (rtConfig.rtTime ());
  ui->rtActivationTemperatureBox->setValue (rtConfig.activationTemperature ());
  ui->rtActivationTimeBox->setValue (rtConfig.activationTime ());

  ThermocyclingConfiguration thermocyclingConfig = protocol.thermocyclingConfig ();
  ui->thermocyclingDenaturationTemperatureBox->setValue (thermocyclingConfig.denaturationTemperature ());
  ui->thermocyclingDenaturationTimeBox->setValue (thermocyclingConfig.denaturationTime ());
  ui->thermocyclingAnnealingTemperatureBox->setValue (thermocyclingConfig.annealingTemperature ());
  ui->thermocyclingAnnealingTimeBox->setValue (thermocyclingConfig.annealingTime ());
  ui->thermocyclingCyclesBox->setValue (thermocyclingConfig.numOfCycles ());
}

PcrProtocol MainWindow::protocol (void) const
{ PreHbhConfiguration preHbhConfig (
    ui->preHbhVelocityBox->value (),
    ui->preHbhTimeBox->value (),
    ui->preHbhAccelerationBox->value (),
    ui->preHbhDecelerationBox->value (),
    MixingConfiguration (
      ui->preHbhMixingNumOfCyclesBox->value (),
      ui->preHbhMixingIntervalBox->value (),
      ui->preHbhMixingVelocityBox->value (),
      ui->preHbhMixingAccelerationBox->value (),
      ui->preHbhMixingDecelerationBox->value ()
    )
  );

  SamplePrepConfiguration samplePrepConfig (
    ui->samplePrepEnableBox->isChecked (),
    qRound (ui->samplePrepPwmFreqBox->value () * 1000.0),
    ui->samplePrepHeatingDurationBox->value ()
  );

  MeteringConfiguration meteringConfig (
    ui->meteringEvacuationVelocityBox->value (),
    ui->meteringEvacuationTimeBox->value (),
    ui->meteringMeteringVelocityBox->value (),
    ui->meteringMeteringTimeBox->value (),
    ui->meteringAccelerationBox->value (),
    ui->meteringDecelerationBox->value ()
  );

  ResuspensionConfiguration resuspensionConfig (
    ui->resuspensionVelocityBox->value (),
    ui->resuspensionTimeBox->value (),
    ui->resuspensionAccelerationBox->value (),
    ui->resuspensionDecelerationBox->value (),
    MixingConfiguration (
      ui->resuspensionMixingNumOfCyclesBox->value (),
      ui->resuspensionMixingIntervalBox->value (),
      ui->resuspensionMixingVelocityBox->value (),
      ui->resuspensionMixingAccelerationBox->value (),
      ui->resuspensionMixingDecelerationBox->value ()
    )
  );

  FillingConfiguration fillingConfig (
    ui->fillingVelocityBox->value (),
    ui->fillingTimeBox->value (),
    ui->fillingAccelerationBox->value (),
    ui->fillingDecelerationBox->value (),
    ui->fillingSealingTemperatureBox->value (),
    ui->fillingSealingTimeBox->value (),
    ui->fillingSealingReverseBox->isChecked ()
  );

  RtConfiguration rtConfig (
    ui->rtRtEnableBox->isChecked (),
    ui->rtRtTemperatureBox->value (),
    ui->rtRtTimeBox->value (),
    ui->rtActivationTemperatureBox->value (),
    ui->rtActivationTimeBox->value ());

  ThermocyclingConfiguration thermocyclingConfig (
    ui->thermocyclingCyclesBox->value (),
    ui->thermocyclingDenaturationTemperatureBox->value (),
    ui->thermocyclingDenaturationTimeBox->value (),
    ui->thermocyclingAnnealingTemperatureBox->value (),
    ui->thermocyclingAnnealingTimeBox->value ());

  return PcrProtocol (preHbhConfig, samplePrepConfig, meteringConfig, resuspensionConfig, fillingConfig, rtConfig, thermocyclingConfig);
}

bool MainWindow::save (const QString &fileName)
{ QFile file (fileName);
  if (!file.open (QFile::WriteOnly | QFile::Truncate)) return false;

  QByteArray data = QJsonDocument (protocol ().toJson ()).toJson ();
  int length = file.write (data);
  file.close ();

  if (length != data.length ()) return false;

  setWindowTitle ("FastPCRConfig2 - " + fileName);
  return true;
}
