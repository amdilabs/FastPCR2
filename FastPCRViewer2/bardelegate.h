#ifndef BARDELEGATE_H
#define BARDELEGATE_H

#include <QStyledItemDelegate>

class BarDelegate : public QStyledItemDelegate
{ Q_OBJECT

  public:
    explicit BarDelegate (QObject *parent = nullptr);

    void paint (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // BARDELEGATE_H
