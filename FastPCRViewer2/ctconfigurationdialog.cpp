#include "ctconfigurationdialog.h"
#include "ui_ctconfigurationdialog.h"

CtConfigurationDialog::CtConfigurationDialog (const CtConfiguration &config, QWidget *parent) : QDialog (parent), ui (new Ui::CtConfigurationDialog)
{ ui->setupUi (this);
  ui->thresholdBox->setValue (config.threshold ());
  ui->autoThresholdBox->setChecked (config.autoThreshold ());
  ui->autoThresholdMultiplierBox->setValue (config.autoThresholdMultiplier ());
  ui->artifactBox->setValue (config.artifact ());
  ui->rawProfileGainBox->setValue (config.rawProfileGain ());
  ui->rawProfileOffsetBox->setValue (config.rawProfileOffset () * 1000.0);
}

CtConfigurationDialog::~CtConfigurationDialog (void)
{ delete ui;
}

CtConfiguration CtConfigurationDialog::configuration (void) const
{ return CtConfiguration (
    ui->thresholdBox->value (),
    ui->autoThresholdBox->isChecked (),
    ui->autoThresholdMultiplierBox->value (),
    ui->artifactBox->value (),
    ui->rawProfileGainBox->value (),
    ui->rawProfileOffsetBox->value () / 1000.0
  );
}
