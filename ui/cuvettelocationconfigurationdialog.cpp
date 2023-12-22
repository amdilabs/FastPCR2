#include "cuvettelocationconfigurationdialog.h"
#include "ui_cuvettelocationconfigurationdialog.h"

CuvetteLocationConfigurationDialog::CuvetteLocationConfigurationDialog (const CuvetteLocationConfigurationContainer &config, QWidget *parent) : QDialog (parent), ui (new Ui::CuvetteLocationConfigurationDialog)
{ ui->setupUi (this);
  ui->tableView->setConfiguration (config);
}

CuvetteLocationConfigurationDialog::~CuvetteLocationConfigurationDialog (void)
{ delete ui;
}

CuvetteLocationConfigurationContainer CuvetteLocationConfigurationDialog::configuration (void) const
{ return ui->tableView->configuration ();
}
