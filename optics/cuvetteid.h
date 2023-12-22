#ifndef CUVETTEID_H
#define CUVETTEID_H

#include <QString>

class CuvetteId
{ public:
    CuvetteId (void);
    CuvetteId (const int channel, const int cuvette);

    bool isValid (void) const;

    int channel (void) const;
    int cuvette (void) const;

    bool match (const int channelMask, const int cuvetteMask) const;
    QString name (void) const;

    bool operator < (const CuvetteId &other) const;

  private:
    int m_channel;
    int m_cuvette;
};

#endif // CUVETTEID_H
