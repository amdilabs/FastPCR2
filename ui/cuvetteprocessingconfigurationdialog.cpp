#include "cuvetteprocessingconfigurationdialog.h"
#include "ui_cuvetteprocessingconfigurationdialog.h"

CuvetteProcessingConfigurationDialog::CuvetteProcessingConfigurationDialog (const CuvetteProcessingConfiguration &config, QWidget *parent) : QDialog (parent), ui (new Ui::CuvetteProcessingConfigurationDialog)
{ ui->setupUi (this);

  ui->algorithmBox->addItem ("Gaussian curve fit", QVariant (CuvetteProcessingConfiguration::GaussianCurveFit));
  ui->algorithmBox->addItem ("Integration", QVariant (CuvetteProcessingConfiguration::Integration));
  ui->algorithmBox->addItem ("Peak amplitude", QVariant (CuvetteProcessingConfiguration::PeakAmplitude));

  ui->algorithmBox->setCurrentIndex (ui->algorithmBox->findData (config.algorithm ()));
  ui->autoOffsetBox->setChecked (config.autoCuvetteOffsetAdjust ());
  ui->biasCorrectBox->setChecked (config.biasCorrect ());
}

CuvetteProcessingConfigurationDialog::~CuvetteProcessingConfigurationDialog (void)
{ delete ui;
}

CuvetteProcessingConfiguration CuvetteProcessingConfigurationDialog::configuration (void) const
{ return CuvetteProcessingConfiguration (ui->algorithmBox->currentData ().value<CuvetteProcessingConfiguration::Algorithm> (), ui->autoOffsetBox->isChecked (), ui->biasCorrectBox->isChecked ());
}
