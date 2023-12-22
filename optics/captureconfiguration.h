#ifndef CAPTURECONFIGURATION_H
#define CAPTURECONFIGURATION_H

#include <QJsonArray>
#include "captureitem.h"

class CaptureConfiguration
{ public:
    CaptureConfiguration (void);
    CaptureConfiguration (const QJsonArray &array);

    QList<CaptureItem> items (void) const;
    void setItems (const QList<CaptureItem> &items);

    int expectedDataSize (void) const;

    QJsonArray toJson (void) const;

  private:
    QList<CaptureItem> m_items;
};

#endif // CAPTURECONFIGURATION_H
