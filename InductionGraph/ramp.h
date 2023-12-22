#ifndef RAMP_H
#define RAMP_H

class Ramp
{ public:
    Ramp (void);
    Ramp (const double origin, const double rate);

    double origin (void) const;
    double rate (void) const;

  private:
    double m_origin;
    double m_rate;
};

#endif // RAMP_H
