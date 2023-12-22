#include "cuvetteid.h"

CuvetteId::CuvetteId (void) : m_channel (-1), m_cuvette (-1)
{
}

CuvetteId::CuvetteId (const int channel, const int cuvette) : m_channel (channel), m_cuvette (cuvette)
{
}

bool CuvetteId::isValid (void) const
{ return (m_channel >= 0) && (m_cuvette >= 0);
}

int CuvetteId::channel (void) const
{ return m_channel;
}

int CuvetteId::cuvette (void) const
{ return m_cuvette;
}

bool CuvetteId::match (const int channelMask, const int cuvetteMask) const
{ if (!isValid ()) return false;
  return ((channelMask & (1 << m_channel)) != 0) && ((cuvetteMask & (1 << m_cuvette)) != 0);
}

QString CuvetteId::name (void) const
{ return "Cuvette " + (m_cuvette >= 0?QString::number (m_cuvette + 1):"?") + "-" + (m_channel >= 0?QString::number (m_channel + 1):"?");
}

bool CuvetteId::operator < (const CuvetteId &other) const
{ if (!isValid ()) return false;
  if (!other.isValid ()) return false;

  if (m_channel < other.m_channel) return true;
  if ((m_channel == other.m_channel) && (m_cuvette < other.m_cuvette)) return true;

  return false;
}
