#ifndef RESUSPENSIONCONFIGURATION_H
#define RESUSPENSIONCONFIGURATION_H

#include <QJsonObject>
#include "mixingconfiguration.h"

class ResuspensionConfiguration
{ public:
    ResuspensionConfiguration (void);
    ResuspensionConfiguration (const double velocity, double time, const double acceleration, const double deceleration, const MixingConfiguration &mixing);
    ResuspensionConfiguration (const QJsonObject &object);

    double velocity (void) const;
    double time (void) const;
    double acceleration (void) const;
    double deceleration (void) const;
    MixingConfiguration mixing (void) const;

    void setVelocity (const double velocity);
    void setTime (const double time);
    void setAcceleration (const double acceleration);
    void setDeceleration (const double deceleration);
    void setMixing (const MixingConfiguration &mixing);

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
    MixingConfiguration m_mixing;
};

#endif // RESUSPENSIONCONFIGURATION_H
