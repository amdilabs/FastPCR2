#ifndef CTCONFIGURATIONDIALOG_H
#define CTCONFIGURATIONDIALOG_H

#include <QDialog>
#include "../optics/ctconfiguration.h"

namespace Ui
{ class CtConfigurationDialog;
}

class CtConfigurationDialog : public QDialog
{ Q_OBJECT

  public:
    explicit CtConfigurationDialog (const CtConfiguration &config, QWidget *parent = nullptr);
    ~CtConfigurationDialog (void);

    CtConfiguration configuration (void) const;

  private:
    Ui::CtConfigurationDialog *ui;
};

#endif // CTCONFIGURATIONDIALOG_H
