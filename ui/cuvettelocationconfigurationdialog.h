#ifndef CUVETTELOCATIONCONFIGURATIONDIALOG_H
#define CUVETTELOCATIONCONFIGURATIONDIALOG_H

#include <QDialog>
#include "../optics/cuvettelocationconfigurationcontainer.h"

namespace Ui
{ class CuvetteLocationConfigurationDialog;
}

class CuvetteLocationConfigurationDialog : public QDialog
{ Q_OBJECT

  public:
    explicit CuvetteLocationConfigurationDialog (const CuvetteLocationConfigurationContainer &config, QWidget *parent = nullptr);
    ~CuvetteLocationConfigurationDialog (void);

    CuvetteLocationConfigurationContainer configuration (void) const;

  private:
    Ui::CuvetteLocationConfigurationDialog *ui;
};

#endif // CUVETTELOCATIONCONFIGURATIONDIALOG_H
