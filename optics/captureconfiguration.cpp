#include "captureconfiguration.h"

CaptureConfiguration::CaptureConfiguration (void)
{
}

CaptureConfiguration::CaptureConfiguration (const QJsonArray &array)
{ for (const QJsonValue &value : array) m_items.append (value.toObject ());
}

QList<CaptureItem> CaptureConfiguration::items (void) const
{ return m_items;
}

void CaptureConfiguration::setItems (const QList<CaptureItem> &items)
{ m_items = items;
}

int CaptureConfiguration::expectedDataSize (void) const
{ int result = 0;
  for (const CaptureItem &item : m_items) result += item.expectedDataSize ();
  return result;
}

QJsonArray CaptureConfiguration::toJson (void) const
{ QJsonArray array;
  for (const CaptureItem &item : m_items) array.append (item.toJson ());
  return array;
}
