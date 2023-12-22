#ifndef RAWDATA_H
#define RAWDATA_H

#include <QIODevice>
#include <QSharedData>

class RawData
{ public:
    RawData (void);
    RawData (const int timestamp, const int diskIndex, const int optoSync, const int adcCount, uint16_t *adc, const int encoderCount, uint16_t *encoder);
    RawData (QIODevice *device);

    bool isValid (void) const;

    int timestamp (void) const;
    int diskIndex (void) const;
    int optoSync (void) const;
    uint16_t *adc (void) const;
    uint16_t *encoder (void) const;

    int size (void) const;
    bool write (QIODevice *device) const;

  private:
    class Data;

    bool m_valid;
    int m_timestamp;
    int m_diskIndex;
    int m_optoSync;
    int m_adcBytes;
    int m_encoderBytes;
    QSharedDataPointer<Data> m_data;
};

class RawData::Data : public QSharedData
{ public:
    Data (void);
    Data (uint16_t *adc, uint16_t *encoder);
    ~Data (void);

    uint16_t *adc (void) const;
    uint16_t *encoder (void) const;

  private:
    uint16_t *m_adc;
    uint16_t *m_encoder;
};

#endif // RAWDATA_H
