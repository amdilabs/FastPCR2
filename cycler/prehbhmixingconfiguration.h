#ifndef PREHBHMIXINGCONFIGURATION_H
#define PREHBHMIXINGCONFIGURATION_H

#include <QJsonObject>

class PreHbhMixingConfiguration
{ public:
    PreHbhMixingConfiguration (void);
    PreHbhMixingConfiguration (const int numOfCycles, const double interval, const double velocity, const double acceleration, const double deceleration);
    PreHbhMixingConfiguration (const QJsonObject &object);

    int numOfCycles (void) const;
    double interval (void) const;
    double velocity (void) const;
    double acceleration (void) const;
    double deceleration (void) const;

    void setNumOfCycles (const int numOfCycles);
    void setInterval (const double interval);
    void setVelocity (const double velocity);
    void setAcceleration (const double acceleration);
    void setDeceleration (const double deceleration);

    QJsonObject toJson (void) const;

  private:
    static const int defaultNumOfCycles;
    static const double defaultInterval;
    static const double defaultVelocity;
    static const double defaultAcceleration;
    static const double defaultDeceleration;

    int m_numOfCycles;
    double m_interval;
    double m_velocity;
    double m_acceleration;
    double m_deceleration;
};

#endif // PREHBHMIXINGCONFIGURATION_H
