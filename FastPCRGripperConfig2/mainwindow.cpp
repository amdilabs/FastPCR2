#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent), ui (new Ui::MainWindow), m_interface (&m_log), m_pge (&m_interface)
{ ui->setupUi (this);

  ui->gripperBox->addItem ("Heater 65", QVariant (Pge::Heater65));
  ui->gripperBox->addItem ("Heater 95", QVariant (Pge::Heater95));
  ui->gripperBox->addItem ("Heatsealer", QVariant (Pge::HeatSealer));

  connect (&m_log, &Log::logReceived, this, &MainWindow::logReceived);
  connect (ui->gripperBox, &QComboBox::activated, this, &MainWindow::gripperBoxActivated);
  connect (ui->readButton, &QPushButton::clicked, this, &MainWindow::readButtonClicked);
  connect (ui->writeButton, &QPushButton::clicked, this, &MainWindow::writeButtonClicked);

  QStringList devices = Interface::listDevices ();
  if (!devices.isEmpty ())
  { if (m_interface.open (devices.first ()))
    { ui->gripperBox->setEnabled (true);

      ui->position1Box->setEnabled (true);
      ui->force1Box->setEnabled (true);
      ui->speed1Box->setEnabled (true);

      ui->position2Box->setEnabled (true);
      ui->force2Box->setEnabled (true);
      ui->speed2Box->setEnabled (true);

      ui->position3Box->setEnabled (true);
      ui->force3Box->setEnabled (true);
      ui->speed3Box->setEnabled (true);

      ui->position4Box->setEnabled (true);
      ui->force4Box->setEnabled (true);
      ui->speed4Box->setEnabled (true);

      ui->readButton->setEnabled (true);
      ui->writeButton->setEnabled (true);

      read ();
    }
  }
}

MainWindow::~MainWindow (void)
{ m_interface.close ();
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

void MainWindow::gripperBoxActivated (int index)
{ Q_UNUSED(index)
  read ();
}

void MainWindow::readButtonClicked (void)
{ read ();
}

void MainWindow::writeButtonClicked (void)
{ if (!write ()) QMessageBox::critical (this, "Error", "Write error.");
}

void MainWindow::read (void)
{ Pge::Select select = ui->gripperBox->currentData ().value<Pge::Select> ();

  ui->position1Box->setValue (m_pge.getIoGroupPosition (select, Pge::Group1) * 100.0);
  ui->force1Box->setValue (m_pge.getIoGroupForce (select, Pge::Group1) * 100.0);
  ui->speed1Box->setValue (m_pge.getIoGroupSpeed (select, Pge::Group1) * 100.0);

  ui->position2Box->setValue (m_pge.getIoGroupPosition (select, Pge::Group2) * 100.0);
  ui->force2Box->setValue (m_pge.getIoGroupForce (select, Pge::Group2) * 100.0);
  ui->speed2Box->setValue (m_pge.getIoGroupSpeed (select, Pge::Group2) * 100.0);

  ui->position3Box->setValue (m_pge.getIoGroupPosition (select, Pge::Group3) * 100.0);
  ui->force3Box->setValue (m_pge.getIoGroupForce (select, Pge::Group3) * 100.0);
  ui->speed3Box->setValue (m_pge.getIoGroupSpeed (select, Pge::Group3) * 100.0);

  ui->position4Box->setValue (m_pge.getIoGroupPosition (select, Pge::Group4) * 100.0);
  ui->force4Box->setValue (m_pge.getIoGroupForce (select, Pge::Group4) * 100.0);
  ui->speed4Box->setValue (m_pge.getIoGroupSpeed (select, Pge::Group4) * 100.0);
}

bool MainWindow::write (void)
{ Pge::Select select = ui->gripperBox->currentData ().value<Pge::Select> ();

  if (!m_pge.setIoGroupPosition (select, Pge::Group1, ui->position1Box->value () / 100.0)) return false;
  if (!m_pge.setIoGroupForce (select, Pge::Group1, ui->force1Box->value () / 100.0)) return false;
  if (!m_pge.setIoGroupSpeed (select, Pge::Group1, ui->speed1Box->value () / 100.0)) return false;

  if (!m_pge.setIoGroupPosition (select, Pge::Group2, ui->position2Box->value () / 100.0)) return false;
  if (!m_pge.setIoGroupForce (select, Pge::Group2, ui->force2Box->value () / 100.0)) return false;
  if (!m_pge.setIoGroupSpeed (select, Pge::Group2, ui->speed2Box->value () / 100.0)) return false;

  if (!m_pge.setIoGroupPosition (select, Pge::Group3, ui->position3Box->value () / 100.0)) return false;
  if (!m_pge.setIoGroupForce (select, Pge::Group3, ui->force3Box->value () / 100.0)) return false;
  if (!m_pge.setIoGroupSpeed (select, Pge::Group3, ui->speed3Box->value () / 100.0)) return false;

  if (!m_pge.setIoGroupPosition (select, Pge::Group4, ui->position4Box->value () / 100.0)) return false;
  if (!m_pge.setIoGroupForce (select, Pge::Group4, ui->force4Box->value () / 100.0)) return false;
  if (!m_pge.setIoGroupSpeed (select, Pge::Group4, ui->speed4Box->value () / 100.0)) return false;

  return m_pge.saveParameter (select);
}
