#ifndef OPTICSCONFIGURATION_H
#define OPTICSCONFIGURATION_H

#include <QJsonObject>

class OpticsConfiguration
{ public:
    enum Algorithm {GaussianCurveFit, Integration, PeakAmplitude};

    OpticsConfiguration (void);
    OpticsConfiguration (const QJsonObject &object);

    int numOfCuvettes (void) const;
    double cuvetteWidth (void) const;
    double cuvetteSpacing (void) const;
    double cuvetteOffset (const int channel) const;
    double darkStart (const int channel) const;
    double darkEnd (const int channel) const;
    Algorithm algorithm (void) const;
    bool autoCuvetteOffsetAdjust (void) const;
    bool biasCorrect (void) const;

    void setNumOfCuvettes (const int numOfCuvettes);
    void setCuvetteWidth (const double cuvetteWidth);
    void setCuvetteSpacing (const double cuvetteSpacing);
    void setCuvetteOffset (const int channel, const double cuvetteOffset);
    void setDarkStart (const int channel, const double darkStart);
    void setDarkEnd (const int channel, const double darkEnd);
    void setAlgorithm (const Algorithm algorithm);
    void setAutoCuvetteOffsetAdjust (const bool autoCuvetteOffsetAdjust);
    void setBiasCorrect (const bool biasCorrect);

    double cuvetteLocation (const int channel, const int cuvette, const double offset = 0.0) const;
    double cuvetteStart (const int channel, const int cuvette, const double offset = 0.0) const;
    double cuvetteEnd (const int channel, const int cuvette, const double offset = 0.0) const;

    QJsonObject toJson (void) const;

  private:
    static const int defaultNumOfCuvettes;
    static const double defaultCuvetteWidth;
    static const double defaultCuvetteSpacing;
    static const double defaultCuvetteOffset [];
    static const double defaultDarkStart [];
    static const double defaultDarkEnd [];
    static const Algorithm defaultAlgorithm;
    static const bool defaultAutoCuvetteOffsetAdjust;
    static const bool defaultBiasCorrect;

    int m_numOfCuvettes;
    double m_cuvetteWidth;
    double m_cuvetteSpacing;
    double m_cuvetteOffset [4];
    double m_darkStart [4];
    double m_darkEnd [4];
    Algorithm m_algorithm;
    bool m_autoCuvetteOffsetAdjust;
    bool m_biasCorrect;
};

#endif // OPTICSCONFIGURATION_H
