#include <QFileDialog>
#include <QMessageBox>
#include "../../lib/settings.h"
#include "../optics/cuvetteid.h"
#include "../optics/cuvetteintensities.h"
#include "../ui/cuvettelocationconfigurationdialog.h"
#include "../ui/cuvetteprocessingconfigurationdialog.h"
#include "ctconfigurationdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent), ui (new Ui::MainWindow)
{ ui->setupUi (this);

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
  m_smoothNpChannelBoxes.append (ui->smoothNpChannelPd0Box);
  m_smoothNpChannelBoxes.append (ui->smoothNpChannelPd1Box);
  m_smoothNpChannelBoxes.append (ui->smoothNpChannelPd2Box);
  m_smoothNpChannelBoxes.append (ui->smoothNpChannelPd3Box);
  m_smoothNpCuvetteBoxes.append (ui->smoothNpCuvette0Box);
  m_smoothNpCuvetteBoxes.append (ui->smoothNpCuvette1Box);
  m_smoothNpCuvetteBoxes.append (ui->smoothNpCuvette2Box);
  m_smoothNpCuvetteBoxes.append (ui->smoothNpCuvette3Box);
  m_smoothNpCuvetteBoxes.append (ui->smoothNpCuvette4Box);
  m_smoothNpCuvetteBoxes.append (ui->smoothNpCuvette5Box);
  m_smoothNpCuvetteBoxes.append (ui->smoothNpCuvette6Box);
  m_smoothNpCuvetteBoxes.append (ui->smoothNpCuvette7Box);
  m_timeDataChannelBoxes.append (ui->timeDataChannelPd0Box);
  m_timeDataChannelBoxes.append (ui->timeDataChannelPd1Box);
  m_timeDataChannelBoxes.append (ui->timeDataChannelPd2Box);
  m_timeDataChannelBoxes.append (ui->timeDataChannelPd3Box);
  m_spatialDataChannelBoxes.append (ui->spatialDataChannelPd0Box);
  m_spatialDataChannelBoxes.append (ui->spatialDataChannelPd1Box);
  m_spatialDataChannelBoxes.append (ui->spatialDataChannelPd2Box);
  m_spatialDataChannelBoxes.append (ui->spatialDataChannelPd3Box);

  restoreSettings ();

  ui->pcrGraphWidget->setChannelMask (maskFromUi (m_pcrGraphChannelBoxes));
  ui->pcrGraphWidget->setCuvetteMask (maskFromUi (m_pcrGraphCuvetteBoxes));
  ui->smoothNpWidget->setChannelMask (maskFromUi (m_smoothNpChannelBoxes));
  ui->smoothNpWidget->setCuvetteMask (maskFromUi (m_smoothNpCuvetteBoxes));
  ui->timeDataWidget->setChannelMask (maskFromUi (m_timeDataChannelBoxes));
  ui->spatialDataWidget->setChannelMask (maskFromUi (m_spatialDataChannelBoxes));
  ui->spatialDataWidget->setCuvetteConfig (m_cuvetteConfig);

  for (int channel = 0; channel < 4; channel++)
  { connect (m_pcrGraphChannelBoxes.at (channel), &QCheckBox::clicked, this, &MainWindow::pcrGraphChannelBoxClicked);
    connect (m_smoothNpChannelBoxes.at (channel), &QCheckBox::clicked, this, &MainWindow::smoothNpChannelBoxClicked);
    connect (m_timeDataChannelBoxes.at (channel), &QCheckBox::clicked, this, &MainWindow::timeDataChannelBoxClicked);
    connect (m_spatialDataChannelBoxes.at (channel), &QCheckBox::clicked, this, &MainWindow::spatialDataChannelBoxClicked);
  }

  for (int cuvette = 0; cuvette < 8; cuvette++)
  { connect (m_pcrGraphCuvetteBoxes.at (cuvette), &QCheckBox::clicked, this, &MainWindow::pcrGraphCuvetteBoxClicked);
    connect (m_smoothNpCuvetteBoxes.at (cuvette), &QCheckBox::clicked, this, &MainWindow::smoothNpCuvetteBoxClicked);
  }

  connect (ui->actionFileOpen, &QAction::triggered, this, &MainWindow::actionFileOpenTriggered);
  connect (ui->actionFileExportTimeData, &QAction::triggered, this, &MainWindow::actionFileExportTimeDataTriggered);
  connect (ui->actionFileExportSpatialData, &QAction::triggered, this, &MainWindow::actionFileExportSpatialDataTriggered);
  connect (ui->actionFileExportCt, &QAction::triggered, this, &MainWindow::actionFileExportCtTriggered);
  connect (ui->actionFileExportProfile, &QAction::triggered, this, &MainWindow::actionFileExportProfileTriggered);
  connect (ui->actionConfigurationCuvette, &QAction::triggered, this, &MainWindow::actionConfigurationCuvetteTriggered);
  connect (ui->actionConfigurationProcessing, &QAction::triggered, this, &MainWindow::actionConfigurationProcessingTriggered);
  connect (ui->actionConfigurationCt, &QAction::triggered, this, &MainWindow::actionConfigurationCtTriggered);
  connect (ui->timeDataCycleBox, &QSpinBox::editingFinished, this, &MainWindow::timeDataCycleBoxEditingFinished);
  connect (ui->spatialDataCycleBox, &QSpinBox::editingFinished, this, &MainWindow::spatialDataCycleBoxEditingFinished);
}

MainWindow::~MainWindow (void)
{ saveSettings ();
  delete ui;
}

void MainWindow::actionFileOpenTriggered (void)
{ QString fileName = QFileDialog::getOpenFileName (this, "Open file", QString (), "PCR files (*.pcr)");
  if (!fileName.isEmpty ())
  {  
  	if (m_file.load (fileName))
    { m_cuvetteConfig = m_file.cuvetteLocationConfig ();
      ui->spatialDataWidget->setCuvetteConfig (m_cuvetteConfig);

      // Set channels for the table view
      QList<int> channels;
      int numOfChannels = m_cuvetteConfig.count();
      for (int index=0; index<numOfChannels; index++) channels.append(index);
      m_values.setChannels(channels);

      process ();

      ui->temperatureWidget->plot (m_file.temperatures ());

      QString text;
      for (const QPair<QString, QString> &p : m_file.userInput ()) text.append (p.first + ": " + p.second + "\n");
      ui->userInputEdit->setPlainText (text);

      ui->timeDataCycleBox->setMaximum (m_file.numOfCycles ());
      ui->timeDataWidget->plot (m_file.timeData (ui->timeDataCycleBox->value ()));

      ui->spatialDataCycleBox->setMaximum (m_file.numOfCycles ());
      ui->spatialDataWidget->plot (m_file.spatialData (ui->spatialDataCycleBox->value ()));

      ui->actionFileExportTimeData->setEnabled (true);
      ui->actionFileExportSpatialData->setEnabled (true);
      ui->actionFileExportCt->setEnabled (true);
      ui->actionFileExportProfile->setEnabled (true);
      m_fileName = fileName;
    }
  }
}

void MainWindow::actionFileExportTimeDataTriggered (void)
{ QFileInfo info (m_fileName);
  QString fileName = QFileDialog::getSaveFileName (this, "Export raw data (time domain)", info.absolutePath () + QDir::separator () + info.completeBaseName () + "_raw_time_cycle" + QString::number (ui->timeDataCycleBox->value ()) + ".csv", "CSV files (*.csv)");

  if (!fileName.isEmpty ())
  { if (!exportTimeData (fileName, m_file.timeData (ui->timeDataCycleBox->value ()))) QMessageBox::critical (this, "Error", "Cannot open file '" + fileName + "'.");
  }
}

void MainWindow::actionFileExportSpatialDataTriggered (void)
{ QFileInfo info (m_fileName);
  QString fileName = QFileDialog::getSaveFileName (this, "Export raw data (spatial domain)", info.absolutePath () + QDir::separator () + info.completeBaseName () + "_raw_spatial_cycle" + QString::number (ui->spatialDataCycleBox->value ()) + ".csv", "CSV files (*.csv)");

  if (!fileName.isEmpty ())
  { if (!exportSpatialData (fileName, m_file.spatialData (ui->spatialDataCycleBox->value ()))) QMessageBox::critical (this, "Error", "Cannot open file '" + fileName + "'.");
  }
}

void MainWindow::actionFileExportCtTriggered (void)
{ QFileInfo info (m_fileName);
  QString fileName = QFileDialog::getSaveFileName (this, "Export CT counts)", info.absolutePath () + QDir::separator () + info.completeBaseName () + "_ct.csv", "CSV files (*.csv)");

  if (!fileName.isEmpty ())
  { if (!exportCt (fileName)) QMessageBox::critical (this, "Error", "Cannot open file '" + fileName + "'.");
  }
}

void MainWindow::actionFileExportProfileTriggered (void)
{ QFileInfo info (m_fileName);
  QString fileName = QFileDialog::getSaveFileName (this, "Export profile", info.absolutePath () + QDir::separator () + info.completeBaseName () + "_profile.csv", "CSV files (*.csv)");

  if (!fileName.isEmpty ())
  { if (!exportProfile (fileName)) QMessageBox::critical (this, "Error", "Cannot open file '" + fileName + "'.");
  }
}

void MainWindow::actionConfigurationCuvetteTriggered (void)
{ CuvetteLocationConfigurationDialog dialog (m_cuvetteConfig, this);
  if (dialog.exec () == CuvetteLocationConfigurationDialog::Accepted)
  { m_cuvetteConfig = dialog.configuration ();
    ui->spatialDataWidget->setCuvetteConfig (m_cuvetteConfig);
    process ();
  }
}

void MainWindow::actionConfigurationProcessingTriggered (void)
{ CuvetteProcessingConfigurationDialog dialog (m_processingConfig, this);
  if (dialog.exec () == CuvetteProcessingConfigurationDialog::Accepted)
  { m_processingConfig = dialog.configuration ();
    process ();
  }
}

void MainWindow::actionConfigurationCtTriggered (void)
{ CtConfigurationDialog dialog (m_ctConfig, this);
  if (dialog.exec () == CtConfigurationDialog::Accepted)
  { m_ctConfig = dialog.configuration ();
    ui->smoothNpWidget->setThreshold (m_ctConfig.threshold ());
    processCt ();
  }
}

void MainWindow::pcrGraphChannelBoxClicked (void)
{ ui->pcrGraphWidget->setChannelMask (maskFromUi (m_pcrGraphChannelBoxes));
}

void MainWindow::pcrGraphCuvetteBoxClicked (void)
{ ui->pcrGraphWidget->setCuvetteMask (maskFromUi (m_pcrGraphCuvetteBoxes));
}

void MainWindow::smoothNpChannelBoxClicked (void)
{ QTextStream (stdout) << "smoothNpChannelBoxClicked" << Qt::endl;
  ui->smoothNpWidget->setChannelMask (maskFromUi (m_smoothNpChannelBoxes));
}

void MainWindow::smoothNpCuvetteBoxClicked (void)
{ QTextStream (stdout) << "smoothNpCuvetteBoxClicked" << Qt::endl;
  ui->smoothNpWidget->setCuvetteMask (maskFromUi (m_smoothNpCuvetteBoxes));
}

void MainWindow::timeDataChannelBoxClicked (void)
{ ui->timeDataWidget->setChannelMask (maskFromUi (m_timeDataChannelBoxes));
}

void MainWindow::timeDataCycleBoxEditingFinished (void)
{ ui->timeDataWidget->plot (m_file.timeData (ui->timeDataCycleBox->value ()));
}

void MainWindow::spatialDataChannelBoxClicked (void)
{ ui->spatialDataWidget->setChannelMask (maskFromUi (m_spatialDataChannelBoxes));
}

void MainWindow::spatialDataCycleBoxEditingFinished (void)
{ ui->spatialDataWidget->plot (m_file.spatialData (ui->spatialDataCycleBox->value ()));
}

void MainWindow::process (void)
{ QMap<CuvetteId, PcrProfile> profiles;
  m_values.clear ();
  for (int cycle = 1; cycle <= m_file.numOfCycles (); cycle++)
  { QList<double> valuesPerCycle;
    for (const SpatialData &data : m_file.spatialData (cycle))  // 4 channels
    { CuvetteIntensities intensities (data, m_cuvetteConfig.configuration (data.channel ()), m_processingConfig);
      QList<double> values = intensities.values ();  // 8 cuvette
      for (int cuvette = 0; cuvette < intensities.numOfCuvettes (); cuvette++) // 8 cuvettes
      { CuvetteId id (data.channel (), cuvette);
        if (!profiles.contains (id)) profiles.insert (id, PcrProfile (id));
        profiles [id].append (values.at (cuvette));
        valuesPerCycle.append(values.at (cuvette));  // Append values of each cuvette
      }
    }

    // Create m_values for table view tab
    m_values.append(valuesPerCycle, m_file.rawData(cycle).timestamp()); // Append 32 cuvette values each cycle
  }

  m_profiles = profiles.values ();
  ui->pcrGraphWidget->plot (m_profiles);    // PCR Profile, graphic view
  ui->tableView->setValues(m_values);       // PCR Profile, Table view

  processCt ();
}

void MainWindow::processCt (void)
{ m_cts = Ct::process (m_profiles, m_ctConfig);
  ui->smoothNpWidget->plot (m_cts);
  ui->ctTable->setValues (m_cts);
}

void MainWindow::restoreSettings (void)
{ QJsonObject object = Settings::restore ("FastPCRViewer2_1").object ();
  //m_cuvetteConfig = CuvetteLocationConfigurationContainer (object.value ("cuvetteConfiguration").toArray ());
  m_processingConfig = CuvetteProcessingConfiguration (object.value ("processingConfiguration").toObject ());
  m_ctConfig = CtConfiguration (object.value ("ctConfiguration").toObject ());
  maskToUi (object.value ("pcrGraphChannelMask").toInt (15), m_pcrGraphChannelBoxes);
  maskToUi (object.value ("pcrGraphCuvetteMask").toInt (255), m_pcrGraphCuvetteBoxes);
  maskToUi (object.value ("smoothNpChannelMask").toInt (15), m_smoothNpChannelBoxes);
  maskToUi (object.value ("smoothNpCuvetteMask").toInt (255), m_smoothNpCuvetteBoxes);
  maskToUi (object.value ("timeDataChannelMask").toInt (15), m_timeDataChannelBoxes);
  maskToUi (object.value ("spatialDataChannelMask").toInt (15), m_spatialDataChannelBoxes);
}

void MainWindow::saveSettings (void)
{ QJsonObject object;
  //object.insert ("cuvetteConfiguration", m_cuvetteConfig.toJson ());
  object.insert ("processingConfiguration", m_processingConfig.toJson ());
  object.insert ("ctConfiguration", m_ctConfig.toJson ());
  object.insert ("pcrGraphChannelMask", maskFromUi (m_pcrGraphChannelBoxes));
  object.insert ("pcrGraphCuvetteMask", maskFromUi (m_pcrGraphCuvetteBoxes));
  object.insert ("smoothNpChannelMask", maskFromUi (m_smoothNpChannelBoxes));
  object.insert ("smoothNpCuvetteMask", maskFromUi (m_smoothNpCuvetteBoxes));
  object.insert ("timeDataChannelMask", maskFromUi (m_timeDataChannelBoxes));
  object.insert ("spatialDataChannelMask", maskFromUi (m_spatialDataChannelBoxes));
  Settings::save ("FastPCRViewer2_1", QJsonDocument (object));
}

bool MainWindow::exportTimeData (const QString &fileName, const QList<TimeData> &data)
{ QFile file (fileName);
  if (!file.open (QFile::WriteOnly | QFile::Truncate | QFile::Text)) return false;

  int maxRows = 0;

  QStringList header;
  for (const TimeData &d : data)
  { QString pd = "PD" + QString::number (d.channel () + 1) + " ";
    header.append (pd + "Time (ms)");
    header.append (pd + "Encoder (ticks)");
    header.append (pd + "ADC (mV)");
    maxRows = qMax (maxRows, d.numOfSamples ());
  }
  file.write (header.join (",").toUtf8 () + "\n");

  for (int row = 0; row < maxRows; row++)
  { QStringList line;

    for (int column = 0; column < data.count (); column++)
    { TimeData d = data.at (column);

      if (row < d.numOfSamples ())
      { line.append (QString::number (static_cast<double> (row) / 100.0, 'f', 3));
        line.append (QString::number (d.encoder ().at (row)));
        line.append (QString::number (d.adc ().at (row) * 1000, 'f', 1));
      } else
      { line.append (QString ());
        line.append (QString ());
        line.append (QString ());
      }
    }

    file.write (line.join (",").toUtf8 () + "\n");
  }

  file.close ();
  return true;
}

bool MainWindow::exportSpatialData (const QString &fileName, const QList<SpatialData> &data)
{ QFile file (fileName);
  if (!file.open (QFile::WriteOnly | QFile::Truncate | QFile::Text)) return false;

  int maxRows = 0;

  QStringList header;
  for (const SpatialData &d : data)
  { QString pd = "PD" + QString::number (d.channel () + 1) + " ";
    header.append (pd + "Position (°)");
    header.append (pd + "ADC (mV)");
    maxRows = qMax (maxRows, d.numOfSamples ());
  }
  file.write (header.join (",").toUtf8 () + "\n");

  for (int row = 0; row < maxRows; row++)
  { QStringList line;

    for (int column = 0; column < data.count (); column++)
    { SpatialData d = data.at (column);

      if (row < d.numOfSamples ())
      { line.append (QString::number (d.position ().at (row), 'f', 3));
        line.append (QString::number (d.adc ().at (row) * 1000, 'f', 1));
      } else
      { line.append (QString ());
        line.append (QString ());
      }
    }

    file.write (line.join (",").toUtf8 () + "\n");
  }

  file.close ();
  return true;
}

bool MainWindow::exportCt (const QString &fileName)
{ QFile file (fileName);
  if (!file.open (QFile::WriteOnly | QFile::Truncate | QFile::Text)) return false;

  QStringList header = QStringList () << "Cuvette" << "CT count" << "Breakpoint" << "Baseline (avg)" << "Baseline (stddev)";
  file.write (header.join (',').toUtf8 () + "\n");

  for (const Ct &ct : m_cts)
  { QStringList line = QStringList () << ct.id ().name () << QString::number (ct.ctCount (), 'f', 3) << QString::number (ct.breakpoint ()) << QString::number (ct.baselineAverage (), 'f', 1) << QString::number (ct.baselineStddev (), 'f', 3);
    file.write (line.join (",").toUtf8 () + "\n");
  }

  file.close ();
  return true;
}

bool MainWindow::exportProfile (const QString &fileName)
{ QFile file (fileName);
  if (!file.open (QFile::WriteOnly | QFile::Truncate | QFile::Text)) return false;

  int maxRows = 0;

  QStringList header;
  for (const Ct &ct : m_cts) { header.append (ct.id ().name () + " (raw profile)"); maxRows = qMax (maxRows, ct.numOfCycles ()); }
  for (const Ct &ct : m_cts) header.append (ct.id ().name () + " (net profile)");
  for (const Ct &ct : m_cts) header.append (ct.id ().name () + " (smooth np)");
  file.write (header.join (',').toUtf8 () + "\n");

  for (int row = 0; row < maxRows; row++)
  { QStringList line;
    for (const Ct &ct : m_cts) line.append (row < ct.numOfCycles ()?QString::number (ct.rawProfile ().at (row), 'f', 1):QString ());
    for (const Ct &ct : m_cts) line.append (row < ct.numOfCycles ()?QString::number (ct.netProfile ().at (row), 'f', 1):QString ());
    for (const Ct &ct : m_cts) line.append (row < ct.numOfCycles ()?QString::number (ct.smoothNp ().at (row), 'f', 1):QString ());
    file.write (line.join (',').toUtf8 () + "\n");
  }

  return true;
}

int MainWindow::maskFromUi (const QList<QCheckBox *> &boxes)
{ int result = 0;
  for (int bit = 0; bit < boxes.count (); bit++) if (boxes.at (bit)->isChecked ()) result |= (1 << bit);
  return result;
}

void MainWindow::maskToUi (const int mask, const QList<QCheckBox *> &boxes)
{ for (int bit = 0; bit < boxes.count (); bit++) boxes.at (bit)->setChecked ((mask & (1 << bit)) != 0);
}
