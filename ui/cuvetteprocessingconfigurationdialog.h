#ifndef CUVETTEPROCESSINGCONFIGURATIONDIALOG_H
#define CUVETTEPROCESSINGCONFIGURATIONDIALOG_H

#include <QDialog>
#include "../optics/cuvetteprocessingconfiguration.h"

namespace Ui
{ class CuvetteProcessingConfigurationDialog;
}

class CuvetteProcessingConfigurationDialog : public QDialog
{ Q_OBJECT

  public:
    explicit CuvetteProcessingConfigurationDialog (const CuvetteProcessingConfiguration &config, QWidget *parent = nullptr);
    ~CuvetteProcessingConfigurationDialog (void);

    CuvetteProcessingConfiguration configuration (void) const;

  private:
    Ui::CuvetteProcessingConfigurationDialog *ui;
};

#endif // CUVETTEPROCESSINGCONFIGURATIONDIALOG_H
