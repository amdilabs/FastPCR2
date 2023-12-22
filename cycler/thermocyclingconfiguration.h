#ifndef THERMOCYCLINGCONFIGURATION_H
#define THERMOCYCLINGCONFIGURATION_H

#include <QJsonObject>

class ThermocyclingConfiguration
{ public:
    ThermocyclingConfiguration (void);
    ThermocyclingConfiguration (const int numOfCycles, const double denaturationTemperature, const double denaturationTime, const double annealingTemperature, const double annealingTime);
    ThermocyclingConfiguration (const QJsonObject &object);

    int numOfCycles (void) const;
    double denaturationTemperature (void) const;
    double denaturationTime (void) const;
    double annealingTemperature (void) const;
    double annealingTime (void) const;

    void setNumOfCycles (const int numOfCycles);
    void setDenaturationTemperature (const double denaturationTemperature);
    void setDenaturationTime (const double denaturationTime);
    void setAnnealingTemperature (const double annealingTemperature);
    void setAnnealingTime (const double annealingTime);

    QJsonObject toJson (void) const;

  private:
    static const int defaultNumOfCycles;
    static const double defaultDenaturationTemperature;
    static const double defaultDenaturationTime;
    static const double defaultAnnealingTemperature;
    static const double defaultAnnealingTime;

    int m_numOfCycles;
    double m_denaturationTemperature;
    double m_denaturationTime;
    double m_annealingTemperature;
    double m_annealingTime;
};

#endif // THERMOCYCLINGCONFIGURATION_H
