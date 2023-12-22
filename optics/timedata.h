#ifndef TIMEDATA_H
#define TIMEDATA_H

#include <QList>
#include "captureconfiguration.h"
#include "rawdata.h"

class TimeData
{ public:
    TimeData (void);

    int timestamp (void) const;
    int numOfSamples (void) const;
    int channel (void) const;
    QList<double> adc (void) const;
    QList<int> encoder (void) const;

    static QList<TimeData> process (const RawData &data, const CaptureConfiguration &config);

  private:
    static const double adcGain;

    int m_timestamp;
    int m_channel;
    QList<double> m_adc;
    QList<int> m_encoder;

    TimeData (const CaptureItem &item, const int timestamp, const int diskIndex, const int optoSync, const int offset, const int stride, uint16_t *adc, uint16_t *encoder);

    static QList<int> channels (const int channelMask);
};

#endif // TIMEDATA_H
