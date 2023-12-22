#ifndef CUVETTELOCATIONCONFIGURATIONTABLEMODEL_H
#define CUVETTELOCATIONCONFIGURATIONTABLEMODEL_H

#include <QAbstractTableModel>
#include "../optics/cuvettelocationconfigurationcontainer.h"

class CuvetteLocationConfigurationTableModel : public QAbstractTableModel
{ Q_OBJECT

  public:
    explicit CuvetteLocationConfigurationTableModel (QObject *parent = nullptr);

    CuvetteLocationConfigurationContainer configuration (void) const;
    void setConfiguration (const CuvetteLocationConfigurationContainer &config);

    int columnCount (const QModelIndex &parent = QModelIndex ()) const override;
    int rowCount (const QModelIndex &parent = QModelIndex ()) const override;
    QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags (const QModelIndex &index) const override;
    bool setData (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

  private:
    CuvetteLocationConfigurationContainer m_config;
};

#endif // CUVETTELOCATIONCONFIGURATIONTABLEMODEL_H
