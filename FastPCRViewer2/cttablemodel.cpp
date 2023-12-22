#include "cttablemodel.h"

CtTableModel::CtTableModel (QObject *parent) : QAbstractTableModel (parent)
{
}

void CtTableModel::setValues (const QList<Ct> &ct)
{ beginResetModel ();
  m_ct = ct;
  endResetModel ();
}

int CtTableModel::columnCount (const QModelIndex &parent) const
{ Q_UNUSED(parent)
  return 6;
}

int CtTableModel::rowCount (const QModelIndex &parent) const
{ Q_UNUSED(parent)
  return m_ct.count ();
}

QVariant CtTableModel::data (const QModelIndex &index, int role) const
{ Ct ct = m_ct.at (index.row ());

  switch (role)
  { case Qt::DisplayRole:
      switch (index.column ())
      { case 0: return ct.id ().name ();
        case 1: return (qIsNaN (ct.ctCount ())?QString ("-"):QString::number (ct.ctCount (), 'f', 1));
        case 2: return QString::number (ct.breakpoint ());
        case 3: return QString::number (ct.baselineAverage (), 'f', 1);
        case 4: return QString::number (ct.baselineStddev (), 'f', 3);
        case 5: return ct.ctCount ();
      }
      break;

    case Qt::TextAlignmentRole: return (index.column () == 0?QVariant (Qt::AlignLeft | Qt::AlignVCenter):QVariant (Qt::AlignRight | Qt::AlignVCenter));
  }

  return QVariant ();
}

QVariant CtTableModel::headerData (int section, Qt::Orientation orientation, int role) const
{ if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
  { switch (section)
    { case 0: return "Cuvette";
      case 1: return "CT count";
      case 2: return "Breakpoint";
      case 3: return "Baseline (avg)";
      case 4: return "Baseline (stdev)";
    }
  }

  return QVariant ();
}
