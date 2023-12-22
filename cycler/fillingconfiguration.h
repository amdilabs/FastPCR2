#ifndef FILLINGCONFIGURATION_H
#define FILLINGCONFIGURATION_H

#include <QJsonObject>

class FillingConfiguration
{ public:
    FillingConfiguration (void);
    FillingConfiguration (const double velocity, const double time, const double acceleration, const double deceleration, const double sealerTemperature, const double sealingTime, const bool reverse);
    FillingConfiguration (const QJsonObject &object);

    double velocity (void) const;
    double time (void) const;
    double acceleration (void) const;
    double deceleration (void) const;
    double sealerTemperature (void) const;
    double sealingTime (void) const;
    bool reverse (void) const;

    void setVelocity (const double velocity);
    void setTime (const double time);
    void setAcceleration (const double acceleration);
    void setDeceleration (const double deceleration);
    void setSealerTemperature (const double sealerTemperature);
    void setSealingTime (const double sealingTime);
    void setSealerEnable(const int sealerMask);
    int sealerEnable (void) const;
    void setReverse (const bool reverse);

    QJsonObject toJson (void) const;

  private:
    static const double defaultVelocity;
    static const double defaultTime;
    static const double defaultAcceleration;
    static const double defaultDeceleration;
    static const double defaultSealerTemperature;
    static const double defaultSealingTime;
    static const int defaultSealerMask;
    static const bool defaultReverse;

    double m_velocity;
    double m_time;
    double m_acceleration;
    double m_deceleration;
    double m_sealerTemperature;
    double m_sealingTime;
    int m_sealerMask;
    bool m_reverse;
};

#endif // FILLINGCONFIGURATION_H
