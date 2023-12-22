#ifndef VALVE_H
#define VALVE_H

#include <QJsonObject>

class Valve
{ public:
    Valve (void);
    Valve (const double angle, const double radius, const double current, const double duration);
    Valve (const QJsonObject &object);

    double angle (void) const;
    double radius (void) const;    
    double current (void) const;
    double duration (void) const;

    void setAngle (const double angle);
    void setRadius (const double radius);
    void setCurrent (const double current);
    void setDuration (const double duration);

    int angleTicks (const double ticksPerRevolution = 16384.0) const;
    int radiusSteps (const double stepsPerMm = 4031.496) const;

    QJsonObject toJson (void) const;

  private:
    static const double defaultAngle;
    static const double defaultRadius;
    static const double defaultCurrent;
    static const double defaultDuration;

    double m_angle;
    double m_radius;
    double m_current;
    double m_duration;
};


#endif // VALVE_H
