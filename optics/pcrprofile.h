#ifndef PCRPROFILE_H
#define PCRPROFILE_H

#include <QList>
#include "cuvetteid.h"

class PcrProfile
{ public:
    PcrProfile (void);
    PcrProfile (const CuvetteId &id);

    CuvetteId id (void) const;
    int numOfCycles (void) const;
    QList<double> values (void) const;
    double at (const int cycle) const;

    void append (const double value);
    void clear (void);

    QList<double> transformed (const double gain, const double offset) const;

  private:
    CuvetteId m_id;
    QList<double> m_values;
};

#endif // PCRPROFILE_H
