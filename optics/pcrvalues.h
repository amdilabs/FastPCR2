#ifndef PCRVALUES_H
#define PCRVALUES_H

#include <QList>

class PcrValues
{ public:
    PcrValues (void);
    PcrValues (const PcrValues &other);
    ~PcrValues (void);

    int numOfCycles (void) const;
    int valuesPerCycle (void) const;

    double value (const int cycle, const int index) const;
    QList<double> values (const int cycle) const;
    int elapsed (const int cycle) const;
    int time (const int cycle) const;

    void append (const QList<double> values, const int elapsed);
    void clear (void);

    QList<int> channels (void) const;
    void setChannels (const QList<int> &channels);

    QString label (const int index) const;

    PcrValues &operator = (const PcrValues &other);

  private:
    static const int capacityInc = 50;

    double *m_values;
    int *m_elapsed;
    int m_capacity;
    int m_numOfCycles;
    int m_valuesPerCycle;
    QList<int> m_channels;

    void copy (const PcrValues &other);
};

#endif // PCRVALUES_H
