#ifndef CUVETTEINTENSITIES_H
#define CUVETTEINTENSITIES_H

#include "../../lib/math/gaussian.h"
#include "cuvettelocationconfiguration.h"
#include "cuvetteprocessingconfiguration.h"
#include "spatialdata.h"

class CuvetteIntensities
{ public:
    CuvetteIntensities (void);
    CuvetteIntensities (const SpatialData &data, const CuvetteLocationConfiguration &locationConfig, const CuvetteProcessingConfiguration &processingConfig);

    int timestamp (void) const;
    int numOfCuvettes (void) const;
    int channel (void) const;
    QList<double> values (void) const;

    QList<Gaussian> gaussian (void) const;
    QList<double> locations (void) const;

  private:
    int m_timestamp;
    int m_channel;
    QList<double> m_values;
    QList<Gaussian> m_gaussian;
    QList<double> m_locations;

    static QList<double> subtract (const QList<double> &list, const double value);
    static double bias (const QList<double> &adc, const QList<double> &position, const double start, const double end);
    static Gaussian gaussian (const QList<double> &adc, const QList<double> &position, const double start, const double end);
    static double findPeak (const QList<double> &adc, const QList<double> &position, const double start, const double end, double *amplitude = nullptr);
    static double integrate (const QList<double> &adc, const QList<double> &position, const double start, const double end);
    static double interp (const double x0, const double x1, const double y0, const double y1, const double x);
    static double area (const double x0, const double x1, const double y0, const double y1, const double start, const double end);
};

#endif // CUVETTEINTENSITIES_H
