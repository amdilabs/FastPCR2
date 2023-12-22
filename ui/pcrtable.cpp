#include "pcrtable.h"

PcrTable::PcrTable (QWidget *parent) : QTableView (parent)
{ setModel (&m_model);
}

void PcrTable::setValues (const PcrValues &values)
{ m_model.setValues (values);
}

void PcrTable::appendValues (const QList<double> &values, const int elapsed)
{ m_model.appendValues (values, elapsed);
}
