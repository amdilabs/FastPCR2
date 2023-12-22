#ifndef METERINGCONFIGURATION_H
#define METERINGCONFIGURATION_H

#include <QJsonObject>

class MeteringConfiguration
{ public:
    MeteringConfiguration (void);
    MeteringConfiguration (const double evacuationVelocity, const double evacuationTime, const double meteringVelocity, const double meteringTime, const double acceleration, const double deceleration);
    MeteringConfiguration (const QJsonObject &object);

    double evacuationVelocity (void) const;
    double evacuationTime (void) const;
    double meteringVelocity (void) const;
    double meteringTime (void) const;
    double acceleration (void) const;
    double deceleration (void) const;

    void setEvacuationVelocity (const double evacuationVelocity);
    void setEvacuationTime (const double evacuationTime);
    void setMeteringVelocity (const double meteringVelocity);
    void setMeteringTime (const double meteringTime);
    void setAcceleration (const double acceleration);
    void setDeceleration (const double deceleration);

    QJsonObject toJson (void) const;

  private:
    static const double defaultEvacuationVelocity;
    static const double defaultEvacuationTime;
    static const double defaultMeteringVelocity;
    static const double defaultMeteringTime;
    static const double defaultAcceleration;
    static const double defaultDeceleration;

    double m_evacuationVelocity;
    double m_evacuationTime;
    double m_meteringVelocity;
    double m_meteringTime;
    double m_acceleration;
    double m_deceleration;
};

#endif // METERINGCONFIGURATION_H
