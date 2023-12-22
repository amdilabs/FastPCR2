#include "pcrvalues.h"

PcrValues::PcrValues (void) : m_capacity (0), m_numOfCycles (0), m_valuesPerCycle (0)
{
}

PcrValues::PcrValues (const PcrValues &other) : m_values (nullptr)
{ copy (other);
}

PcrValues::~PcrValues (void)
{ if (m_capacity)
  { delete [] m_values;
    delete [] m_elapsed;
  }
}

int PcrValues::numOfCycles (void) const
{ return m_numOfCycles;
}

int PcrValues::valuesPerCycle (void) const
{ return m_valuesPerCycle;
}

double PcrValues::value (const int cycle, const int index) const
{ return m_values [cycle * m_valuesPerCycle + index];
}

QList<double> PcrValues::values (const int cycle) const
{ QList<double> values;
  values.reserve (m_valuesPerCycle);

  double *ptr = &m_values [cycle * m_valuesPerCycle];
  for (int index = 0; index < m_valuesPerCycle; index++) values.append (*ptr++);

  return values;
}

int PcrValues::elapsed (const int cycle) const
{ return m_elapsed [cycle];
}

int PcrValues::time (const int cycle) const
{ return (cycle > 0?m_elapsed [cycle] - m_elapsed [cycle - 1]:m_elapsed [0]);
}

void PcrValues::append (const QList<double> values, const int elapsed)
{ if (m_numOfCycles == m_capacity)
  { if (!m_capacity)
    { if (values.isEmpty ()) return;

      m_capacity = capacityInc;
      m_valuesPerCycle = values.count ();

      m_values = new double [m_capacity * m_valuesPerCycle];
      m_elapsed = new int [m_capacity];
    } else
    { m_capacity += capacityInc;

      double *v = new double [m_capacity * m_valuesPerCycle];
      memcpy (v, m_values, m_numOfCycles * m_valuesPerCycle * sizeof (double));

      delete [] m_values;
      m_values = v;

      int *e = new int [m_capacity];
      memcpy (e, m_elapsed, m_numOfCycles * sizeof (int));

      delete [] m_elapsed;
      m_elapsed = e;
    }
  }

  double *ptr = &m_values [m_numOfCycles * m_valuesPerCycle];
  for (int index = 0; index < m_valuesPerCycle; index++) *ptr++ = (index < values.count ()?values.at (index):qSNaN ());

  m_elapsed [m_numOfCycles] = elapsed;

  m_numOfCycles++;
}

void PcrValues::clear (void)
{ if (m_capacity)
  { delete [] m_values;
    delete [] m_elapsed;
  }

  m_capacity = 0;
  m_numOfCycles = 0;
  m_valuesPerCycle = 0;
}

QList<int> PcrValues::channels (void) const
{ return m_channels;
}

void PcrValues::setChannels (const QList<int> &channels)
{ m_channels = channels;
}

QString PcrValues::label (const int index) const
{ if (m_channels.isEmpty ()) return "Cuvette " + QString::number (index + 1) + "-?";

  int numOfCuvettes = m_valuesPerCycle / m_channels.count ();
  return "Cuvette " + QString::number ((index % numOfCuvettes) + 1) + "-" + QString::number ((m_channels.at (index / numOfCuvettes)) + 1);
}

PcrValues &PcrValues::operator = (const PcrValues &other)
{ if (this != &other) copy (other);
  return *this;
}

void PcrValues::copy (const PcrValues &other)
{ clear ();

  m_capacity = other.m_capacity;
  m_numOfCycles = other.m_numOfCycles;
  m_valuesPerCycle = other.m_valuesPerCycle;
  m_channels = other.m_channels;

  if (other.m_capacity)
  { m_values = new double [m_capacity * m_valuesPerCycle];
    memcpy (m_values, other.m_values, m_numOfCycles * m_valuesPerCycle * sizeof (double));

    m_elapsed = new int [m_capacity];
    memcpy (m_elapsed, other.m_elapsed, m_numOfCycles * sizeof (int));
  }
}
