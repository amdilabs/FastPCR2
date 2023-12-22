#ifndef RTCONFIGURATION_H
#define RTCONFIGURATION_H

#include <QJsonObject>

class RtConfiguration
{ public:
    RtConfiguration (void);
    RtConfiguration (const bool rtEnable, const double rtTemperature, const double rtTime, const double activationTemperature, const double activationTime);
    RtConfiguration (const QJsonObject &object);

    bool rtEnable (void) const;
    double rtTemperature (void) const;
    double rtTime (void) const;
    double activationTemperature (void) const;
    double activationTime (void) const;

    void setRtEnable (const bool rtEnable);
    void setRtTemperature (const double rtTemperature);
    void setRtTime (const double rtTime);
    void setActivationTemperature (const double activationTemperature);
    void setActivationTime (const double activationTime);

    QJsonObject toJson (void) const;

  private:
    static const bool defaultRtEnable;
    static const double defaultRtTemperature;
    static const double defaultRtTime;
    static const double defaultActivationTemperature;
    static const double defaultActivationTime;

    bool m_rtEnable;
    double m_rtTemperature;
    double m_rtTime;
    double m_activationTemperature;
    double m_activationTime;
};

#endif // RTCONFIGURATION_H
