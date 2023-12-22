#ifndef CUVETTELOCATIONCONFIGURATIONTABLE_H
#define CUVETTELOCATIONCONFIGURATIONTABLE_H

#include <QTableView>
#include "cuvettelocationconfigurationtablemodel.h"

class CuvetteLocationConfigurationTable : public QTableView
{ Q_OBJECT

  public:
    explicit CuvetteLocationConfigurationTable (QWidget *parent = nullptr);

    CuvetteLocationConfigurationContainer configuration (void) const;
    void setConfiguration (const CuvetteLocationConfigurationContainer &config);

  private:
    CuvetteLocationConfigurationTableModel m_model;
};

#endif // CUVETTELOCATIONCONFIGURATIONTABLE_H
