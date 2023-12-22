#include "diskconfigurationdialog.h"
#include "ui_diskconfigurationdialog.h"

DiskConfigurationDialog::DiskConfigurationDialog (const DiskConfiguration &config, QWidget *parent) : QDialog (parent), ui (new Ui::DiskConfigurationDialog)
{ ui->setupUi (this);
  ui->cuvetteOffsetBox->setValue (config.cuvetteOffset ());
  ui->cuvetteSeparationBox->setValue (config.cuvetteSeparation ());
  ui->heater65LocationBox->setValue (config.heater65Location ());
  ui->heater95LocationBox->setValue (config.heater95Location ());

  ui->hyperbaricValveAngleBox->setValue (config.hyperbaricValve ().angle ());
  ui->hyperbaricValveRadiusBox->setValue (config.hyperbaricValve ().radius ());
  ui->hyperbaricValveCurrentBox->setValue (config.hyperbaricValve ().current ());
  ui->hyperbaricValveDurationBox->setValue (config.hyperbaricValve ().duration ());

  ui->meteringValveAngleBox->setValue (config.meteringValve ().angle ());
  ui->meteringValveRadiusBox->setValue (config.meteringValve ().radius ());
  ui->meteringValveCurrentBox->setValue (config.meteringValve ().current ());
  ui->meteringValveDurationBox->setValue (config.meteringValve ().duration ());

  ui->insertValveAngleBox->setValue (config.insertValve ().angle ());
  ui->insertValveRadiusBox->setValue (config.insertValve ().radius ());
  ui->insertValveCurrentBox->setValue (config.insertValve ().current ());
  ui->insertValveDurationBox->setValue (config.insertValve ().duration ());
}

DiskConfigurationDialog::~DiskConfigurationDialog (void)
{ delete ui;
}

DiskConfiguration DiskConfigurationDialog::configuration (void) const
{ return DiskConfiguration (
    ui->cuvetteOffsetBox->value (),
    ui->cuvetteSeparationBox->value (),
    ui->heater65LocationBox->value (),
    ui->heater95LocationBox->value (),
    Valve (ui->hyperbaricValveAngleBox->value (), ui->hyperbaricValveRadiusBox->value (), ui->hyperbaricValveCurrentBox->value (), ui->hyperbaricValveDurationBox->value ()),
    Valve (ui->meteringValveAngleBox->value (), ui->meteringValveRadiusBox->value (), ui->meteringValveCurrentBox->value (), ui->meteringValveDurationBox->value ()),
    Valve (ui->insertValveAngleBox->value (), ui->insertValveRadiusBox->value (), ui->insertValveCurrentBox->value (), ui->insertValveDurationBox->value ())
  );
}
