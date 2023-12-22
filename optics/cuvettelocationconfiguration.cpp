#include "cuvettelocationconfiguration.h"

const int CuvetteLocationConfiguration::defaultNumOfCuvettes = 8;
const double CuvetteLocationConfiguration::defaultCuvetteWidth = 8.0;
const double CuvetteLocationConfiguration::defaultCuvetteSpacing = 13.39;
const double CuvetteLocationConfiguration::defaultCuvetteOffset = 50.75;
const double CuvetteLocationConfiguration::defaultDarkStart = 300.0;
const double CuvetteLocationConfiguration::defaultDarkEnd = 360.0;

CuvetteLocationConfiguration::CuvetteLocationConfiguration (void) :
  m_numOfCuvettes (defaultNumOfCuvettes),
  m_cuvetteWidth (defaultCuvetteWidth),
  m_cuvetteSpacing (defaultCuvetteSpacing),
  m_cuvetteOffset (defaultCuvetteOffset),
  m_darkStart (defaultDarkStart),
  m_darkEnd (defaultDarkEnd)
{
}

CuvetteLocationConfiguration::CuvetteLocationConfiguration (const QJsonObject &object) :
  m_numOfCuvettes (object.value ("numOfCuvettes").toInt (defaultNumOfCuvettes)),
  m_cuvetteWidth (object.value ("cuvetteWidth").toDouble (defaultCuvetteWidth)),
  m_cuvetteSpacing (object.value ("cuvetteSpacing").toDouble (defaultCuvetteSpacing)),
  m_cuvetteOffset (object.value ("cuvetteOffset").toDouble (defaultCuvetteOffset)),
  m_darkStart (object.value ("darkStart").toDouble (defaultDarkStart)),
  m_darkEnd (object.value ("darkEnd").toDouble (defaultDarkEnd))
{
}

int CuvetteLocationConfiguration::numOfCuvettes (void) const
{ return m_numOfCuvettes;
}

double CuvetteLocationConfiguration::cuvetteWidth (void) const
{ return m_cuvetteWidth;
}

double CuvetteLocationConfiguration::cuvetteSpacing (void) const
{ return m_cuvetteSpacing;
}

double CuvetteLocationConfiguration::cuvetteOffset (void) const
{ return m_cuvetteOffset;
}

double CuvetteLocationConfiguration::darkStart (void) const
{ return m_darkStart;
}

double CuvetteLocationConfiguration::darkEnd (void) const
{ return m_darkEnd;
}

void CuvetteLocationConfiguration::setNumOfCuvettes (const int numOfCuvettes)
{ m_numOfCuvettes = numOfCuvettes;
}

void CuvetteLocationConfiguration::setCuvetteWidth (const double cuvetteWidth)
{ m_cuvetteWidth = cuvetteWidth;
}

void CuvetteLocationConfiguration::setCuvetteSpacing (const double cuvetteSpacing)
{ m_cuvetteSpacing = cuvetteSpacing;
}

void CuvetteLocationConfiguration::setCuvetteOffset (const double cuvetteOffset)
{ m_cuvetteOffset = cuvetteOffset;
}

void CuvetteLocationConfiguration::setDarkStart (const double darkStart)
{ m_darkStart = darkStart;
}

void CuvetteLocationConfiguration::setDarkEnd (const double darkEnd)
{ m_darkEnd = darkEnd;
}

double CuvetteLocationConfiguration::cuvetteLocation (const int cuvette, const double offset) const
{ return m_cuvetteOffset + static_cast<double> (cuvette) * m_cuvetteSpacing + offset;
}

double CuvetteLocationConfiguration::cuvetteStart (const int cuvette, const double offset) const
{ return cuvetteLocation (cuvette, offset) - 0.5 * m_cuvetteWidth;
}

double CuvetteLocationConfiguration::cuvetteEnd (const int cuvette, const double offset) const
{ return cuvetteLocation (cuvette, offset) + 0.5 * m_cuvetteWidth;
}

QJsonObject CuvetteLocationConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("numOfCuvettes", m_numOfCuvettes);
  object.insert ("cuvetteWidth", m_cuvetteWidth);
  object.insert ("cuvetteSpacing", m_cuvetteSpacing);
  object.insert ("cuvetteOffset", m_cuvetteOffset);
  object.insert ("darkStart", m_darkStart);
  object.insert ("darkEnd", m_darkEnd);
  return object;
}
