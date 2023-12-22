#ifndef CTTABLE_H
#define CTTABLE_H

#include <QTableView>
#include "../optics/ct.h"
#include "cttablemodel.h"

class CtTable : public QTableView
{ Q_OBJECT

  public:
    explicit CtTable (QWidget *parent = nullptr);

    void setValues (const QList<Ct> &ct);

  private:
    CtTableModel m_model;
};

#endif // CTTABLE_H
