#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QList>
#include <QPointF>
#include "data.h"
#include "ramp.h"

class Processor
{ public:
    static QList<QPointF> points1090 (const Data &data);
    static QList<QPointF> pointsFromStart (const Data &data, const double step, const double length);
    static QList<QPointF> pointsFromPeak (const Data &data, const double step, const double length);

    static Ramp twoPointRamp (const QList<QPointF> &points);
    static Ramp linearRegressionRamp (const QList<QPointF> &points, const Data &data);

    static double startTime (const Data &data);
    static double peakTime (const Data &data);

  private:
    static int startIndex (const QList<double> &values);
    static int peakIndex (const QList<double> &values);
    static double interp (const double x0, const double x1, const double y0, const double y1, const double x);
    static QPointF find (const double time, const Data &data);
};

#endif // PROCESSOR_H
