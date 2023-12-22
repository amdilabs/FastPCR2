#include "bardelegate.h"
#include "cttable.h"

CtTable::CtTable (QWidget *parent) : QTableView (parent)
{ setModel (&m_model);
  setItemDelegateForColumn (5, new BarDelegate);
}

void CtTable::setValues (const QList<Ct> &ct)
{ m_model.setValues (ct);
  resizeColumnsToContents ();
}
