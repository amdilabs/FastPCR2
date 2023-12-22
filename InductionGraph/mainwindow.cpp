#include <QFileDialog>
#include <QMessageBox>
#include "../../lib/settings.h"
#include "data.h"
#include "processor.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent), ui (new Ui::MainWindow)
{ ui->setupUi (this);

  ui->settingsAlignmentBox->addItem ("None", Configuration::None);
  ui->settingsAlignmentBox->addItem ("Rise start", Configuration::RiseStart);
  ui->settingsAlignmentBox->addItem ("Peak", Configuration::Peak);

  ui->settingsRampDataBox->addItem ("Peak 10 to 90%", Configuration::Peak1090);
  ui->settingsRampDataBox->addItem ("From start", Configuration::FromStart);
  ui->settingsRampDataBox->addItem ("From peak", Configuration::FromPeak);

  ui->settingsRampFittingBox->addItem ("Two point", Configuration::TwoPoint);
  ui->settingsRampFittingBox->addItem ("Linear regression", Configuration::LinearRegression);

  restoreSettings ();
  ui->settingsAlignmentBox->setCurrentIndex (ui->settingsAlignmentBox->findData (m_config.alignment ()));
  ui->settingsRampDataBox->setCurrentIndex (ui->settingsRampDataBox->findData (m_config.rampData ()));
  ui->settingsRampFittingBox->setCurrentIndex (ui->settingsRampFittingBox->findData (m_config.rampFitting ()));
  ui->settingsStepBox->setValue (m_config.step ());
  ui->settingsLengthBox->setValue (m_config.length ());

  connect (ui->actionOpen, &QAction::triggered, this, &MainWindow::actionOpenTriggered);
  connect (ui->actionClear, &QAction::triggered, this, &MainWindow::actionClearTriggered);
  connect (ui->actionClearLast, &QAction::triggered, this, &MainWindow::actionClearLastTriggered);
  connect (ui->actionExportCsv, &QAction::triggered, this, &MainWindow::actionExportCsvTriggered);
  connect (ui->actionExportPdf, &QAction::triggered, this, &MainWindow::actionExportPdfTriggered);
  connect (ui->actionAbout, &QAction::triggered, this, &MainWindow::actionAboutTriggered);
  connect (ui->settingsAlignmentBox, &QComboBox::activated, this, &MainWindow::settingsAlignmentBoxActivated);
  connect (ui->settingsRampDataBox, &QComboBox::activated, this, &MainWindow::settingsRampDataBoxActivated);
  connect (ui->settingsRampFittingBox, &QComboBox::activated, this, &MainWindow::settingsRampFittingBoxActivated);
  connect (ui->settingsStepBox, &QDoubleSpinBox::editingFinished, this, &MainWindow::settingsStepBoxEditingFinished);
  connect (ui->settingsLengthBox, &QDoubleSpinBox::editingFinished, this, &MainWindow::settingsLengthBoxEditingFinished);
}

MainWindow::~MainWindow (void)
{ saveSettings ();
  delete ui;
}

void MainWindow::actionOpenTriggered (void)
{ QStringList fileNames = QFileDialog::getOpenFileNames (this, "Open file(s)", QString (), "CSV-files (*.csv)");
  if (!fileNames.isEmpty ())
  { for (const QString &fileName : fileNames)
    { Data data (fileName);
      if (data.isValid ())
      { plot (data);
        m_data.append (data);
      }
    }
  }
}

void MainWindow::actionClearTriggered (void)
{ ui->widget->clear ();
  m_data.clear ();
}

void MainWindow::actionClearLastTriggered (void)
{ ui->widget->clearLast ();
  if (!m_data.isEmpty ()) m_data.removeLast ();
}

void MainWindow::actionExportCsvTriggered (void)
{ QString fileName = QFileDialog::getSaveFileName (this, "Export CSV", QString (), "CSV-files (*.csv)");
  if (!fileName.isEmpty ())
  { QFile file (fileName);
    if (file.open (QFile::WriteOnly | QFile::Truncate | QFile::Text))
    { QStringList names;
      for (const Data &data : m_data)
      { names.append (data.name ());
        names.append (data.name ());
      }
      file.write (names.join (',').toUtf8 () + '\n');

      double maxOffset = 0.0;

      QStringList header;
      for (int column = 0; column < m_data.count (); column++)
      { maxOffset = qMax (maxOffset, offset (m_data.at (column), m_config.alignment ()));
        header.append ("Time (s)");
        header.append ("Temperature (°C)");
      }
      file.write (header.join (',').toUtf8 () + '\n');

      for (int row = 0;; row++)
      { bool done = true;
        QStringList values;
        for (const Data &data : m_data)
        { QList<double> time = data.time (offset (data, m_config.alignment ()) - maxOffset);

          if (row < time.count ())
          { done = false;
            values.append (QString::number (time.at (row), 'f', 3));
            values.append (QString::number (data.temperature ().at (row), 'f', 1));
          } else
          { values.append (QString ());
            values.append (QString ());
          }
        }

        if (done) break;
        file.write (values.join (',').toUtf8 () + '\n');
      }

      file.close ();
    } else QMessageBox::critical (this, "Error", "Cannot open file '" + fileName + "'.");
  }
}

void MainWindow::actionExportPdfTriggered (void)
{ QString fileName = QFileDialog::getSaveFileName (this, "Export PDF", QString (), "PDF-files (*.pdf)");
  if (!fileName.isEmpty ())
  { if (!ui->widget->savePdf (fileName)) QMessageBox::critical (this, "Error", "Cannot export PDF.");
  }
}

void MainWindow::actionAboutTriggered (void)
{ QMessageBox::about (this, "About " + QCoreApplication::applicationName (), QCoreApplication::applicationName () + " version " + QCoreApplication::applicationVersion ());
}

void MainWindow::settingsAlignmentBoxActivated (int index)
{ Q_UNUSED(index)
  m_config.setAlignment (ui->settingsAlignmentBox->currentData ().value<Configuration::Alignment> ());
  process ();
}

void MainWindow::settingsRampDataBoxActivated (int index)
{ Q_UNUSED(index)
  m_config.setRampData (ui->settingsRampDataBox->currentData ().value<Configuration::RampData> ());
  process ();
}

void MainWindow::settingsRampFittingBoxActivated (int index)
{ Q_UNUSED(index)
  m_config.setRampFitting (ui->settingsRampFittingBox->currentData ().value<Configuration::RampFitting> ());
  process ();
}

void MainWindow::settingsStepBoxEditingFinished (void)
{ m_config.setStep (ui->settingsStepBox->value ());
  process ();
}

void MainWindow::settingsLengthBoxEditingFinished (void)
{ m_config.setLength (ui->settingsLengthBox->value ());
  process ();
}

void MainWindow::process (void)
{ ui->widget->clear ();
  for (const Data &data : m_data) plot (data);
}

void MainWindow::restoreSettings (void)
{ QJsonObject object = Settings::restore ("InductionGraph").object ();
  m_config = Configuration (object.value ("configuration").toObject ());
}

void MainWindow::saveSettings (void)
{ QJsonObject object;
  object.insert ("configuration", m_config.toJson ());
  Settings::save ("InductionGraph", QJsonDocument (object));
}

void MainWindow::plot (const Data &data)
{ ui->widget->plot (data, offset (data, m_config.alignment ()), ramp (data, points (data, m_config.rampData (), m_config.step (), m_config.length ()), m_config.rampFitting ()));
}

double MainWindow::offset (const Data &data, const Configuration::Alignment alignment)
{ switch (alignment)
  { case Configuration::None: return 0.0;
    case Configuration::RiseStart: return Processor::startTime (data);
    case Configuration::Peak: return Processor::peakTime (data);
  }

  return 0.0;
}

QList<QPointF> MainWindow::points (const Data &data, const Configuration::RampData rampData, const double step, const double length)
{ switch (rampData)
  { case Configuration::Peak1090: return Processor::points1090 (data);
    case Configuration::FromStart: return Processor::pointsFromStart (data, step, length);
    case Configuration::FromPeak: return Processor::pointsFromPeak (data, step, length);
  }

  return QList<QPointF> ();
}

Ramp MainWindow::ramp (const Data &data, const QList<QPointF> &points, const Configuration::RampFitting rampFitting)
{ switch (rampFitting)
  { case Configuration::TwoPoint: return Processor::twoPointRamp (points);
    case Configuration::LinearRegression: return Processor::linearRegressionRamp (points, data);
  }

  return Ramp ();
}
