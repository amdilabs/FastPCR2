#include "ledconfigurationdialog.h"
#include "ui_ledconfigurationdialog.h"

LedConfigurationDialog::LedConfigurationDialog (const LedConfigurationContainer &config, QWidget *parent) : QDialog (parent), ui (new Ui::LedConfigurationDialog)
{ ui->setupUi (this);

  ui->led0CurrentBox->setValue (config.configuration (0).current ());
  ui->led1CurrentBox->setValue (config.configuration (1).current ());
  ui->led2CurrentBox->setValue (config.configuration (2).current ());
  ui->led3CurrentBox->setValue (config.configuration (3).current ());
  ui->led0ShuntBox->setValue (config.configuration (0).shunt () * 1000.0);
  ui->led1ShuntBox->setValue (config.configuration (1).shunt () * 1000.0);
  ui->led2ShuntBox->setValue (config.configuration (2).shunt () * 1000.0);
  ui->led3ShuntBox->setValue (config.configuration (3).shunt () * 1000.0);
}

LedConfigurationDialog::~LedConfigurationDialog (void)
{ delete ui;
}

LedConfigurationContainer LedConfigurationDialog::configuration (void) const
{ LedConfigurationContainer config;
  config.setConfiguration (0, LedConfiguration (ui->led0CurrentBox->value (), ui->led0ShuntBox->value () / 1000.0));
  config.setConfiguration (1, LedConfiguration (ui->led1CurrentBox->value (), ui->led1ShuntBox->value () / 1000.0));
  config.setConfiguration (2, LedConfiguration (ui->led2CurrentBox->value (), ui->led2ShuntBox->value () / 1000.0));
  config.setConfiguration (3, LedConfiguration (ui->led3CurrentBox->value (), ui->led3ShuntBox->value () / 1000.0));
  return config;
}
