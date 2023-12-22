#include "pcrtablemodel.h"

PcrTableModel::PcrTableModel (QObject *parent) : QAbstractTableModel (parent)
{
}

void PcrTableModel::setValues (const PcrValues &values)
{ beginResetModel ();
  m_values = values;
  endResetModel ();
}

void PcrTableModel::appendValues (const QList<double> &values, const int elapsed)
{ if (m_values.numOfCycles () != 0)
  { beginInsertRows (QModelIndex (), m_values.numOfCycles (), m_values.numOfCycles ());
    m_values.append (values, elapsed);
    endInsertRows ();
  } else
  { beginResetModel ();
    m_values.append (values, elapsed);
    endResetModel ();
  }
}

int PcrTableModel::columnCount (const QModelIndex &parent) const
{ Q_UNUSED(parent)
  return m_values.valuesPerCycle () + 3;
}

int PcrTableModel::rowCount (const QModelIndex &parent) const
{ Q_UNUSED(parent)
  return m_values.numOfCycles ();
}

QVariant PcrTableModel::data (const QModelIndex &index, int role) const
{ switch (role)
  { case Qt::DisplayRole:
      switch (index.column ())
      { case 0: return QString::number (index.row () + 1);
        case 1: return QString::number (static_cast<double> (m_values.time (index.row ())) / 1000.0, 'f', 1) + " s";
        case 2: return QString::number (static_cast<double> (m_values.elapsed (index.row ())) / 1000.0, 'f', 1) + " s";
        default: return QString::number (m_values.value (index.row (), index.column () - 3) * 1000.0, 'f', 1) + " mV";
      }

    case Qt::TextAlignmentRole: return QVariant (Qt::AlignRight | Qt::AlignVCenter);
  }

  return QVariant ();
}

QVariant PcrTableModel::headerData (int section, Qt::Orientation orientation, int role) const
{ if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
  { switch (section)
    { case 0: return "Cycle";
      case 1: return "Time";
      case 2: return "Elapsed";
      default: return m_values.label (section - 3);
    }
  }

  return QVariant ();
}
