#ifndef LEDCONFIGURATIONDIALOG_H
#define LEDCONFIGURATIONDIALOG_H

#include <QDialog>
#include "../optics/ledconfigurationcontainer.h"

namespace Ui
{ class LedConfigurationDialog;
}

class LedConfigurationDialog : public QDialog
{ Q_OBJECT

  public:
    explicit LedConfigurationDialog (const LedConfigurationContainer &config, QWidget *parent = nullptr);
    ~LedConfigurationDialog (void);

    LedConfigurationContainer configuration (void) const;

  private:
    Ui::LedConfigurationDialog *ui;
};

#endif // LEDCONFIGURATIONDIALOG_H
