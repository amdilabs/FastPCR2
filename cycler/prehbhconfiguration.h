#ifndef PREHBHCONFIGURATION_H
#define PREHBHCONFIGURATION_H

#include <QJsonObject>
#include "prehbhmixingconfiguration.h"

class PreHbhConfiguration
{ public:
    PreHbhConfiguration (void);
    PreHbhConfiguration (const double velocity, double time, const double acceleration, const double deceleration, const PreHbhMixingConfiguration &mixing);
    PreHbhConfiguration (const QJsonObject &object);

    double velocity (void) const;
    double time (void) const;
    double acceleration (void) const;
    double deceleration (void) const;
    PreHbhMixingConfiguration mixing (void) const;

    void setVelocity (const double velocity);
    void setTime (const double time);
    void setAcceleration (const double acceleration);
    void setDeceleration (const double deceleration);
    void setMixing (const PreHbhMixingConfiguration &mixing);

    QJsonObject toJson (void) const;

  private:
    static const double defaultVelocity;
    static const double defaultTime;
    static const double defaultAcceleration;
    static const double defaultDeceleration;

    double m_velocity;
    double m_time;
    double m_acceleration;
    double m_deceleration;
    PreHbhMixingConfiguration m_mixing;
};

#endif // PREHBHCONFIGURATION_H
