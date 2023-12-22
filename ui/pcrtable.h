#ifndef PCRTABLE_H
#define PCRTABLE_H

#include <QTableView>
#include "pcrtablemodel.h"

class PcrTable : public QTableView
{ Q_OBJECT

  public:
    explicit PcrTable (QWidget *parent = nullptr);

    void setValues (const PcrValues &values);
    void appendValues (const QList<double> &values, const int elapsed);

  private:
    PcrTableModel m_model;
};

#endif // PCRTABLE_H
