#ifndef SPATIALDATA_H
#define SPATIALDATA_H

#include "timedata.h"

class SpatialData
{ public:
    SpatialData (void);
    SpatialData (const TimeData &data);

    int timestamp (void) const;
    int numOfSamples (void) const;
    int channel (void) const;
    QList<double> adc (void) const;
    QList<double> position (void) const;

    static QList<SpatialData> process (const QList<TimeData> &data);

  private:
    static const double ticksPerRotation;

    int m_timestamp;
    int m_channel;
    QList<double> m_adc;
    QList<double> m_position;
};

#endif // SPATIALDATA_H
