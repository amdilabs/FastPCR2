#include <QtGlobal>
#include "ramp.h"

Ramp::Ramp (void) : m_origin (qSNaN ()), m_rate (qSNaN ())
{
}

Ramp::Ramp (const double origin, const double rate) : m_origin (origin), m_rate (rate)
{
}

double Ramp::origin (void) const
{ return m_origin;
}

double Ramp::rate (void) const
{ return m_rate;
}
