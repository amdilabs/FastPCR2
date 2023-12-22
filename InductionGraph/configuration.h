#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QJsonObject>

class Configuration
{ public:
    enum Alignment {None, RiseStart, Peak};
    enum RampData {Peak1090, FromStart, FromPeak};
    enum RampFitting {TwoPoint, LinearRegression};

    Configuration (void);
    Configuration (const QJsonObject &object);

    Alignment alignment (void) const;
    RampData rampData (void) const;
    RampFitting rampFitting (void) const;
    double step (void) const;
    double length (void) const;

    void setAlignment (const Alignment alignment);
    void setRampData (const RampData rampData);
    void setRampFitting (const RampFitting rampFitting);
    void setStep (const double step);
    void setLength (const double length);

    QJsonObject toJson (void) const;

  private:
    static const Alignment defaultAlignment;
    static const RampData defaultRampData;
    static const RampFitting defaultRampFitting;
    static const double defaultStep;
    static const double defaultLength;

    Alignment m_alignment;
    RampData m_rampData;
    RampFitting m_rampFitting;
    double m_step;
    double m_length;

    static Alignment alignmentFromJson (const QJsonValue &value);
    static RampData rampDataFromJson (const QJsonValue &value);
    static RampFitting rampFittingFromJson (const QJsonValue &value);

    static QJsonValue alignmentToJson (const Alignment alignment);
    static QJsonValue rampDataToJson (const RampData rampData);
    static QJsonValue rampFittingToJson (const RampFitting rampFitting);
};

#endif // CONFIGURATION_H
