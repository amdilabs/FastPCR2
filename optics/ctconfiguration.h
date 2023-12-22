#ifndef CTCONFIGURATION_H
#define CTCONFIGURATION_H

#include <QJsonObject>

class CtConfiguration
{ public:
    CtConfiguration (void);
    CtConfiguration (const double threshold, const bool autoThreshold, const double autoThresholdMultiplier, const int artifact, const double rawProfileGain, const double rawProfileOffset);
    CtConfiguration (const QJsonObject &object);

    double threshold (void) const;
    bool autoThreshold (void) const;
    double autoThresholdMultiplier (void) const;
    int artifact (void) const;
    double rawProfileGain (void) const;
    double rawProfileOffset (void) const;

    void setThreshold (const double threshold);
    void setAutoThreshold (const bool autoThreshold);
    void setAutoThresholdMultiplier (const double autoThresholdMultiplier);
    void setArtifact (const int artifact);
    void setRawProfileGain (const double rawProfileGain);
    void setRawProfileOffset (const double rawProfileOffset);

    QJsonObject toJson (void) const;

  private:
    static const double defaultThreshold;
    static const bool defaultAutoThreshold;
    static const double defaultAutoThresholdMultiplier;
    static const int defaultArtifact;
    static const double defaultRawProfileGain;
    static const double defaultRawProfileOffset;

    double m_threshold;
    bool m_autoThreshold;
    double m_autoThresholdMultiplier;
    int m_artifact;
    double m_rawProfileGain;
    double m_rawProfileOffset;
};

#endif // CTCONFIGURATION_H
