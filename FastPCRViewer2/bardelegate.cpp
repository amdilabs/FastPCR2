#include <QPainter>
#include "bardelegate.h"

BarDelegate::BarDelegate(QObject *parent) : QStyledItemDelegate (parent)
{
}

void BarDelegate::paint (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{ double value = index.data (Qt::DisplayRole).toDouble ();
  int width = (qIsNaN (value)?0:qRound ((static_cast<double> (option.rect.width ()) / 50.0) * value));
  painter->fillRect (option.rect.left (), option.rect.top (), width, option.rect.height (), option.palette.highlight ());
}
