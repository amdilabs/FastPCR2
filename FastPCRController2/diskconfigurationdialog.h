#ifndef DISKCONFIGURATIONDIALOG_H
#define DISKCONFIGURATIONDIALOG_H

#include <QDialog>
#include "diskconfiguration.h"

namespace Ui
{ class DiskConfigurationDialog;
}

class DiskConfigurationDialog : public QDialog
{ Q_OBJECT

  public:
    explicit DiskConfigurationDialog (const DiskConfiguration &config, QWidget *parent = nullptr);
    ~DiskConfigurationDialog (void);

    DiskConfiguration configuration (void) const;

  private:
    Ui::DiskConfigurationDialog *ui;
};

#endif // DISKCONFIGURATIONDIALOG_H
