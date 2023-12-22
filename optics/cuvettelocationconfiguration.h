#ifndef CUVETTELOCATIONCONFIGURATION_H
#define CUVETTELOCATIONCONFIGURATION_H

#include <QJsonObject>

class CuvetteLocationConfiguration
{ public:
    CuvetteLocationConfiguration (void);
    CuvetteLocationConfiguration (const QJsonObject &object);

    int numOfCuvettes (void) const;
    double cuvetteWidth (void) const;
    double cuvetteSpacing (void) const;
    double cuvetteOffset (void) const;
    double darkStart (void) const;
    double darkEnd (void) const;

    void setNumOfCuvettes (const int numOfCuvettes);
    void setCuvetteWidth (const double cuvetteWidth);
    void setCuvetteSpacing (const double cuvetteSpacing);
    void setCuvetteOffset (const double cuvetteOffset);
    void setDarkStart (const double darkStart);
    void setDarkEnd (const double darkEnd);

    double cuvetteLocation (const int cuvette, const double offset = 0.0) const;
    double cuvetteStart (const int cuvette, const double offset = 0.0) const;
    double cuvetteEnd (const int cuvette, const double offset = 0.0) const;

    QJsonObject toJson (void) const;

  private:
    static const int defaultNumOfCuvettes;
    static const double defaultCuvetteWidth;
    static const double defaultCuvetteSpacing;
    static const double defaultCuvetteOffset;
    static const double defaultDarkStart;
    static const double defaultDarkEnd;

    int m_numOfCuvettes;
    double m_cuvetteWidth;
    double m_cuvetteSpacing;
    double m_cuvetteOffset;
    double m_darkStart;
    double m_darkEnd;
};

#endif // CUVETTELOCATIONCONFIGURATION_H
