#ifndef DECELERATIONCONFIGURATION_H
#define DECELERATIONCONFIGURATION_H

#include <QJsonObject>

class DecelerationConfiguration
{ public:
    DecelerationConfiguration (void);
    DecelerationConfiguration (const bool enable, const int stepSize, const double interval);
    DecelerationConfiguration (const QJsonObject &object);

    bool enable (void) const;
    int stepSize (void) const;
    double interval (void) const;

    void setEnable (const bool enable);
    void setStepSize (const int stepSize);
    void setInterval (const double interval);

    QJsonObject toJson (void) const;

  private:
    static const bool defaultEnable;
    static const int defaultStepSize;
    static const double defaultInterval;

    bool m_enable;
    int m_stepSize;
    double m_interval;
};

#endif // DECELERATIONCONFIGURATION_H
