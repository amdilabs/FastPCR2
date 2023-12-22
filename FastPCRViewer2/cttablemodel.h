#ifndef CTTABLEMODEL_H
#define CTTABLEMODEL_H

#include <QAbstractTableModel>
#include "../optics/ct.h"

class CtTableModel : public QAbstractTableModel
{ Q_OBJECT

  public:
    explicit CtTableModel (QObject *parent = nullptr);

    void setValues (const QList<Ct> &ct);

    virtual int columnCount (const QModelIndex &parent = QModelIndex ()) const override;
    virtual int rowCount (const QModelIndex &parent = QModelIndex ()) const override;
    virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  private:
    QList<Ct> m_ct;
};

#endif // CTTABLEMODEL_H
