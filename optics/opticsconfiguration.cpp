#include <QJsonArray>
#include "opticsconfiguration.h"

const int OpticsConfiguration::defaultNumOfCuvettes = 8;
const double OpticsConfiguration::defaultCuvetteWidth = 300.0;
const double OpticsConfiguration::defaultCuvetteSpacing = -616.0;
const double OpticsConfiguration::defaultCuvetteOffset [] = {5348.0, 8956.0, 5348.0, 8956.0};
const double OpticsConfiguration::defaultDarkStart [] = {7500.0, 0.0, 7500.0, 0.0};
const double OpticsConfiguration::defaultDarkEnd [] = {10000.0, 2500.0, 10000.0, 2500.0};
const OpticsConfiguration::Algorithm OpticsConfiguration::defaultAlgorithm = OpticsConfiguration::PeakAmplitude;
const bool OpticsConfiguration::defaultAutoCuvetteOffsetAdjust = false;
const bool OpticsConfiguration::defaultBiasCorrect = true;

OpticsConfiguration::OpticsConfiguration (void) :
  m_numOfCuvettes (defaultNumOfCuvettes),
  m_cuvetteWidth (defaultCuvetteWidth),
  m_cuvetteSpacing (defaultCuvetteSpacing),
  m_cuvetteOffset {defaultCuvetteOffset [0], defaultCuvetteOffset [1], defaultCuvetteOffset [2], defaultCuvetteOffset [3]},
  m_darkStart {defaultDarkStart [0], defaultDarkStart [1], defaultDarkStart [2], defaultDarkStart [3]},
  m_darkEnd {defaultDarkEnd [0], defaultDarkEnd [1], defaultDarkEnd [2], defaultDarkEnd [3]},
  m_algorithm (defaultAlgorithm),
  m_autoCuvetteOffsetAdjust (defaultAutoCuvetteOffsetAdjust),
  m_biasCorrect (defaultBiasCorrect)
{
}

OpticsConfiguration::OpticsConfiguration (const QJsonObject &object) :
  m_numOfCuvettes (object.value ("numOfCuvettes").toInt (defaultNumOfCuvettes)),
  m_cuvetteWidth (object.value ("cuvetteWidth").toDouble (defaultCuvetteWidth)),
  m_cuvetteSpacing (object.value ("cuvetteSpacing").toDouble (defaultCuvetteSpacing)),
  m_autoCuvetteOffsetAdjust (object.value ("autoCuvetteOffsetAdjust").toBool (defaultAutoCuvetteOffsetAdjust)),
  m_biasCorrect (object.value ("biasCorrect").toBool (defaultBiasCorrect))
{ QJsonArray cuvetteOffset = object.value ("cuvetteOffset").toArray ();
  QJsonArray darkStart = object.value ("darkStart").toArray ();
  QJsonArray darkEnd = object.value ("darkEnd").toArray ();

  for (int channel = 0; channel < 4; channel++)
  { m_cuvetteOffset [channel] = (channel < cuvetteOffset.count ()?cuvetteOffset.at (channel).toDouble (defaultCuvetteOffset [channel]):defaultCuvetteOffset [channel]);
    m_darkStart [channel] = (channel < darkStart.count ()?darkStart.at (channel).toDouble (defaultDarkStart [channel]):defaultDarkStart [channel]);
    m_darkEnd [channel] = (channel < darkEnd.count ()?darkEnd.at (channel).toDouble (defaultDarkEnd [channel]):defaultDarkEnd [channel]);
  }

  QString str = object.value ("algorithm").toString ();
  if (str == "gaussianCurveFit") m_algorithm = GaussianCurveFit; else
  if (str == "integration") m_algorithm = Integration; else
  if (str == "peakAmplitude") m_algorithm = PeakAmplitude; else
    m_algorithm = defaultAlgorithm;
}

int OpticsConfiguration::numOfCuvettes (void) const
{ return m_numOfCuvettes;
}

double OpticsConfiguration::cuvetteWidth (void) const
{ return m_cuvetteWidth;
}

double OpticsConfiguration::cuvetteSpacing (void) const
{ return m_cuvetteSpacing;
}

double OpticsConfiguration::cuvetteOffset (const int channel) const
{ return m_cuvetteOffset [channel];
}

double OpticsConfiguration::darkStart (const int channel) const
{ return m_darkStart [channel];
}

double OpticsConfiguration::darkEnd (const int channel) const
{ return m_darkEnd [channel];
}

OpticsConfiguration::Algorithm OpticsConfiguration::algorithm (void) const
{ return m_algorithm;
}

bool OpticsConfiguration::autoCuvetteOffsetAdjust (void) const
{ return m_autoCuvetteOffsetAdjust;
}

bool OpticsConfiguration::biasCorrect (void) const
{ return m_biasCorrect;
}

void OpticsConfiguration::setNumOfCuvettes (const int numOfCuvettes)
{ m_numOfCuvettes = numOfCuvettes;
}

void OpticsConfiguration::setCuvetteWidth (const double cuvetteWidth)
{ m_cuvetteWidth = cuvetteWidth;
}

void OpticsConfiguration::setCuvetteSpacing (const double cuvetteSpacing)
{ m_cuvetteSpacing = cuvetteSpacing;
}

void OpticsConfiguration::setCuvetteOffset (const int channel, const double cuvetteOffset)
{ m_cuvetteOffset [channel] = cuvetteOffset;
}

void OpticsConfiguration::setDarkStart (const int channel, const double darkStart)
{ m_darkStart [channel] = darkStart;
}

void OpticsConfiguration::setDarkEnd (const int channel, const double darkEnd)
{ m_darkEnd [channel] = darkEnd;
}

void OpticsConfiguration::setAlgorithm (const Algorithm algorithm)
{ m_algorithm = algorithm;
}

void OpticsConfiguration::setAutoCuvetteOffsetAdjust (const bool autoCuvetteOffsetAdjust)
{ m_autoCuvetteOffsetAdjust = autoCuvetteOffsetAdjust;
}

void OpticsConfiguration::setBiasCorrect (const bool biasCorrect)
{ m_biasCorrect = biasCorrect;
}

double OpticsConfiguration::cuvetteLocation (const int channel, const int cuvette, const double offset) const
{ return m_cuvetteOffset [channel] + static_cast<double> (cuvette) * m_cuvetteSpacing + offset;
}

double OpticsConfiguration::cuvetteStart (const int channel, const int cuvette, const double offset) const
{ return cuvetteLocation (channel, cuvette, offset) - 0.5 * m_cuvetteWidth;
}

double OpticsConfiguration::cuvetteEnd (const int channel, const int cuvette, const double offset) const
{ return cuvetteLocation (channel, cuvette, offset) + 0.5 * m_cuvetteWidth;
}

QJsonObject OpticsConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("numOfCuvettes", m_numOfCuvettes);
  object.insert ("cuvetteWidth", m_cuvetteWidth);
  object.insert ("cuvetteSpacing", m_cuvetteSpacing);

  QJsonArray cuvetteOffset;
  QJsonArray darkStart;
  QJsonArray darkEnd;

  for (int channel = 0; channel < 4; channel++)
  { cuvetteOffset.append (m_cuvetteOffset [channel]);
    darkStart.append (m_darkStart [channel]);
    darkEnd.append (m_darkEnd [channel]);
  }

  object.insert ("cuvetteOffset", cuvetteOffset);
  object.insert ("darkStart", darkStart);
  object.insert ("darkEnd", darkEnd);

  switch (m_algorithm)
  { case GaussianCurveFit: object.insert ("algorithm", "gaussianCurveFit"); break;
    case Integration: object.insert ("algorithm", "integration"); break;
    case PeakAmplitude: object.insert ("algorithm", "peakAmplitude"); break;
  }

  object.insert ("autoCuvetteOffsetAdjust", m_autoCuvetteOffsetAdjust);
  object.insert ("biasCorrect", m_biasCorrect);

  return object;
}
