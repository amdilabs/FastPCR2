#ifndef SAMPLEPREPCONFIGURATION_H
#define SAMPLEPREPCONFIGURATION_H

#include <QJsonObject>

class SamplePrepConfiguration
{ public:
    SamplePrepConfiguration (void);
    SamplePrepConfiguration (const bool enable, const int pwmFreq, const double heatingDuration);
    SamplePrepConfiguration (const QJsonObject &object);

    bool enable (void) const;
    int pwmFreq (void) const;
    double heatingDuration (void) const;

    void setEnable (const bool enable);
    void setPwmFreq (const int pwmFreq);
    void setHeatingDuration (const double heatingDuration);

    QJsonObject toJson (void) const;

  private:
    static const bool defaultEnable;
    static const int defaultPwmFreq;
    static const double defaultHeatingDuration;

    bool m_enable;
    int m_pwmFreq;
    double m_heatingDuration;
};

#endif // SAMPLEPREPCONFIGURATION_H
