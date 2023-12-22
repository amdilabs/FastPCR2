#ifndef LEDCONFIGURATION_H
#define LEDCONFIGURATION_H

#include <QJsonObject>

class LedConfiguration
{ public:
    LedConfiguration (void);
    LedConfiguration (const double current, const double shunt);
    LedConfiguration (const QJsonObject &object);

    double current (void) const;
    double shunt (void) const;

    void setCurrent (const double current);
    void setShunt (const double shunt);

    double vSense (void) const;

    QJsonObject toJson (void) const;

  private:
    static const double defaultCurrent;
    static const double defaultShunt;

    double m_current;
    double m_shunt;
};

#endif // LEDCONFIGURATION_H
