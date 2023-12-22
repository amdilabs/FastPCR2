#include "cuvettelocationconfigurationtablemodel.h"

CuvetteLocationConfigurationTableModel::CuvetteLocationConfigurationTableModel (QObject *parent) : QAbstractTableModel (parent)
{
}

CuvetteLocationConfigurationContainer CuvetteLocationConfigurationTableModel::configuration (void) const
{ return m_config;
}

void CuvetteLocationConfigurationTableModel::setConfiguration (const CuvetteLocationConfigurationContainer &config)
{ beginResetModel ();
  m_config = config;
  m_config.init (15);
  endResetModel ();
}

int CuvetteLocationConfigurationTableModel::columnCount (const QModelIndex &parent) const
{ Q_UNUSED(parent)
  return 7;
}

int CuvetteLocationConfigurationTableModel::rowCount (const QModelIndex &parent) const
{ Q_UNUSED(parent)
  return m_config.count ();
}

QVariant CuvetteLocationConfigurationTableModel::data (const QModelIndex &index, int role) const
{ int channel = m_config.channels ().at (index.row ());
  CuvetteLocationConfiguration config = m_config.configuration (channel);

  switch (role)
  { case Qt::DisplayRole:
      switch (index.column ())
      { case 0: return "PD" + QString::number (channel + 1);
        case 1: return QString::number (config.numOfCuvettes ());
        case 2: return QString::number (config.cuvetteWidth (), 'f', 2) + "°";
        case 3: return QString::number (config.cuvetteSpacing (), 'f', 2) + "°";
        case 4: return QString::number (config.cuvetteOffset (), 'f', 2) + "°";
        case 5: return QString::number (config.darkStart (), 'f', 2) + "°";
        case 6: return QString::number (config.darkEnd (), 'f', 2) + "°";
      }
      break;

    case Qt::EditRole:
      switch (index.column ())
      { case 1: return config.numOfCuvettes ();
        case 2: return config.cuvetteWidth ();
        case 3: return config.cuvetteSpacing ();
        case 4: return config.cuvetteOffset ();
        case 5: return config.darkStart ();
        case 6: return config.darkEnd ();
      }
      break;

    case Qt::TextAlignmentRole:
      return QVariant (index.column () != 0?Qt::AlignRight | Qt::AlignVCenter:Qt::AlignLeft | Qt::AlignVCenter);
  }

  return QVariant ();
}

QVariant CuvetteLocationConfigurationTableModel::headerData (int section, Qt::Orientation orientation, int role) const
{ if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
  { switch (section)
    { case 0: return "Channel";
      case 1: return "#Cuvettes";
      case 2: return "Cuvette width";
      case 3: return "Cuvette spacing";
      case 4: return "Cuvette offset";
      case 5: return "Dark start";
      case 6: return "Dark end";
    }
  }

  return QVariant ();
}

Qt::ItemFlags CuvetteLocationConfigurationTableModel::flags (const QModelIndex &index) const
{ return (index.column () != 0?Qt::ItemIsEditable | QAbstractTableModel::flags (index):QAbstractTableModel::flags (index));
}

bool CuvetteLocationConfigurationTableModel::setData (const QModelIndex &index, const QVariant &value, int role)
{ if (role == Qt::EditRole)
  { int channel = m_config.channels ().at (index.row ());
    CuvetteLocationConfiguration config = m_config.configuration (channel);

    switch (index.column ())
    { case 1: config.setNumOfCuvettes (value.toInt ()); break;
      case 2: config.setCuvetteWidth (value.toDouble ()); break;
      case 3: config.setCuvetteSpacing (value.toDouble ()); break;
      case 4: config.setCuvetteOffset (value.toDouble ()); break;
      case 5: config.setDarkStart (value.toDouble ()); break;
      case 6: config.setDarkEnd (value.toDouble ()); break;
    }

    m_config.setConfiguration (channel, config);
    return true;
  }

  return false;
}
