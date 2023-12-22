#include "pcrprofile.h"

PcrProfile::PcrProfile (void)
{
}

PcrProfile::PcrProfile (const CuvetteId &id) : m_id (id)
{
}

CuvetteId PcrProfile::id (void) const
{ return m_id;
}

int PcrProfile::numOfCycles (void) const
{ return m_values.count ();
}

QList<double> PcrProfile::values (void) const
{ return m_values;
}

double PcrProfile::at (const int cycle) const
{ return ((cycle >= 1) && (cycle <= m_values.count ())?m_values.at (cycle - 1):qSNaN ());
}

void PcrProfile::append (const double value)
{ m_values.append (value);
}

void PcrProfile::clear (void)
{ m_values.clear ();
}

QList<double> PcrProfile::transformed (const double gain, const double offset) const
{ QList<double> result; result.reserve (m_values.count ());
  for (const double value : m_values) result.append (gain * value + offset);
  return result;
}
