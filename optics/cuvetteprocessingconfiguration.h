#ifndef CUVETTEPROCESSINGCONFIGURATION_H
#define CUVETTEPROCESSINGCONFIGURATION_H

#include <QJsonObject>

class CuvetteProcessingConfiguration
{ public:
    enum Algorithm {GaussianCurveFit, Integration, PeakAmplitude};

    CuvetteProcessingConfiguration (void);
    CuvetteProcessingConfiguration (const Algorithm algorithm, const bool autoCuvetteOffsetAdjust, const bool biasCorrect);
    CuvetteProcessingConfiguration (const QJsonObject &object);

    Algorithm algorithm (void) const;
    bool autoCuvetteOffsetAdjust (void) const;
    bool biasCorrect (void) const;

    void setAlgorithm (const Algorithm algorithm);
    void setAutoCuvetteOffsetAdjust (const bool autoCuvetteOffsetAdjust);
    void setBiasCorrect (const bool biasCorret);

    QJsonObject toJson (void) const;

  private:
    static const Algorithm defaultAlgorithm;
    static const bool defaultAutoCuvetteOffsetAdjust;
    static const bool defaultBiasCorrect;

    Algorithm m_algorithm;
    bool m_autoCuvetteOffsetAdjust;
    bool m_biasCorrect;

    static Algorithm algorithmFromJson (const QJsonValue &value);
    static QJsonValue algorithmToJson (const Algorithm algorithm);
};

#endif // CUVETTEPROCESSINGCONFIGURATION_H
