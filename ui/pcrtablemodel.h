#ifndef PCRTABLEMODEL_H
#define PCRTABLEMODEL_H

#include <QAbstractTableModel>
#include "../optics/pcrvalues.h"

class PcrTableModel : public QAbstractTableModel
{ Q_OBJECT

  public:
    explicit PcrTableModel (QObject *parent = nullptr);

    void setValues (const PcrValues &values);
    void appendValues (const QList<double> &values, const int elapsed);

    virtual int columnCount (const QModelIndex &parent = QModelIndex ()) const override;
    virtual int rowCount (const QModelIndex &parent = QModelIndex ()) const override;
    virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  private:
    PcrValues m_values;
};

#endif // PCRTABLEMODEL_H
