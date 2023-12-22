#include "cuvetteprocessingconfiguration.h"

const CuvetteProcessingConfiguration::Algorithm CuvetteProcessingConfiguration::defaultAlgorithm = CuvetteProcessingConfiguration::PeakAmplitude;
const bool CuvetteProcessingConfiguration::defaultAutoCuvetteOffsetAdjust = false;
const bool CuvetteProcessingConfiguration::defaultBiasCorrect = true;

CuvetteProcessingConfiguration::CuvetteProcessingConfiguration (void) :
  m_algorithm (defaultAlgorithm),
  m_autoCuvetteOffsetAdjust (defaultAutoCuvetteOffsetAdjust),
  m_biasCorrect (defaultBiasCorrect)
{
}

CuvetteProcessingConfiguration::CuvetteProcessingConfiguration (const Algorithm algorithm, const bool autoCuvetteOffsetAdjust, const bool biasCorrect) :
  m_algorithm (algorithm),
  m_autoCuvetteOffsetAdjust (autoCuvetteOffsetAdjust),
  m_biasCorrect (biasCorrect)
{
}

CuvetteProcessingConfiguration::CuvetteProcessingConfiguration (const QJsonObject &object) :
  m_algorithm (algorithmFromJson (object.value ("algorithm"))),
  m_autoCuvetteOffsetAdjust (object.value ("autoCuvetteOffsetAdjust").toBool (defaultAutoCuvetteOffsetAdjust)),
  m_biasCorrect (object.value ("biasCorrect").toBool (defaultBiasCorrect))
{
}

CuvetteProcessingConfiguration::Algorithm CuvetteProcessingConfiguration::algorithm (void) const
{ return m_algorithm;
}

bool CuvetteProcessingConfiguration::autoCuvetteOffsetAdjust (void) const
{ return m_autoCuvetteOffsetAdjust;
}

bool CuvetteProcessingConfiguration::biasCorrect (void) const
{ return m_biasCorrect;
}

void CuvetteProcessingConfiguration::setAlgorithm (const Algorithm algorithm)
{ m_algorithm = algorithm;
}

void CuvetteProcessingConfiguration::setAutoCuvetteOffsetAdjust (const bool autoCuvetteOffsetAdjust)
{ m_autoCuvetteOffsetAdjust = autoCuvetteOffsetAdjust;
}

void CuvetteProcessingConfiguration::setBiasCorrect (const bool biasCorrect)
{ m_biasCorrect = biasCorrect;
}

QJsonObject CuvetteProcessingConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("algorithm", algorithmToJson (m_algorithm));
  object.insert ("autoCuvetteOffsetAdjust", m_autoCuvetteOffsetAdjust);
  object.insert ("biasCorrect", m_biasCorrect);
  return object;
}

CuvetteProcessingConfiguration::Algorithm CuvetteProcessingConfiguration::algorithmFromJson (const QJsonValue &value)
{ QString str = value.toString ();
  if (str == "gaussianCurveFit") return GaussianCurveFit;
  if (str == "integration") return Integration;
  if (str == "peakAmplitude") return PeakAmplitude;
  return defaultAlgorithm;
}

QJsonValue CuvetteProcessingConfiguration::algorithmToJson (const Algorithm algorithm)
{ switch (algorithm)
  { case GaussianCurveFit: return QJsonValue ("gaussianCurveFit");
    case Integration: return QJsonValue ("integration");
    case PeakAmplitude: return QJsonValue ("peakAmplitude");
  }

  return algorithmToJson (defaultAlgorithm);
}
