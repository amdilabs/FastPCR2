#include "../../lib/ui/delegate/doublespinboxdelegate.h"
#include "../../lib/ui/delegate/spinboxdelegate.h"
#include "cuvettelocationconfigurationtable.h"

CuvetteLocationConfigurationTable::CuvetteLocationConfigurationTable (QWidget *parent) : QTableView (parent)
{ setModel (&m_model);
  setItemDelegateForColumn (1, new SpinBoxDelegate (1, 99));
  setItemDelegateForColumn (2, new DoubleSpinBoxDelegate (0.01, 360.0, 2, "°"));
  setItemDelegateForColumn (3, new DoubleSpinBoxDelegate (-1440.0, 1440.0, 2, "°"));
  setItemDelegateForColumn (4, new DoubleSpinBoxDelegate (-1440.0, 1440.0, 2, "°"));
  setItemDelegateForColumn (5, new DoubleSpinBoxDelegate (-1440.0, 1440.0, 2, "°"));
  setItemDelegateForColumn (6, new DoubleSpinBoxDelegate (-1440.0, 1440.0, 2, "°"));
}

CuvetteLocationConfigurationContainer CuvetteLocationConfigurationTable::configuration (void) const
{ return m_model.configuration ();
}

void CuvetteLocationConfigurationTable::setConfiguration (const CuvetteLocationConfigurationContainer &config)
{ m_model.setConfiguration (config);
  resizeColumnsToContents ();
}
