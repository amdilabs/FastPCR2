#include "ctconfiguration.h"

const double CtConfiguration::defaultThreshold = 20.0;
const bool CtConfiguration::defaultAutoThreshold = false;
const double CtConfiguration::defaultAutoThresholdMultiplier = 10.0;
const int CtConfiguration::defaultArtifact = 5;
const double CtConfiguration::defaultRawProfileGain = 1.0;
const double CtConfiguration::defaultRawProfileOffset = 0.0;

CtConfiguration::CtConfiguration (void) :
  m_threshold (defaultThreshold),
  m_autoThreshold (defaultAutoThreshold),
  m_autoThresholdMultiplier (defaultAutoThresholdMultiplier),
  m_artifact (defaultArtifact),
  m_rawProfileGain (defaultRawProfileGain),
  m_rawProfileOffset (defaultRawProfileOffset)
{
}

CtConfiguration::CtConfiguration (const double threshold, const bool autoThreshold, const double autoThresholdMultiplier, const int artifact, const double rawProfileGain, const double rawProfileOffset) :
  m_threshold (threshold),
  m_autoThreshold (autoThreshold),
  m_autoThresholdMultiplier (autoThresholdMultiplier),
  m_artifact (artifact),
  m_rawProfileGain (rawProfileGain),
  m_rawProfileOffset (rawProfileOffset)
{
}

CtConfiguration::CtConfiguration (const QJsonObject &object) :
  m_threshold (object.value ("threshold").toDouble (defaultThreshold)),
  m_autoThreshold (object.value ("autoThreshold").toBool (defaultAutoThreshold)),
  m_autoThresholdMultiplier (object.value ("autoThresholdMultiplier").toDouble (defaultAutoThresholdMultiplier)),
  m_artifact (object.value ("artifact").toInt (defaultArtifact)),
  m_rawProfileGain (object.value ("rawProfileGain").toDouble (defaultRawProfileGain)),
  m_rawProfileOffset (object.value ("rawProfileOffset").toDouble (defaultRawProfileOffset))
{
}

double CtConfiguration::threshold (void) const
{ return m_threshold;
}

bool CtConfiguration::autoThreshold (void) const
{ return m_autoThreshold;
}

double CtConfiguration::autoThresholdMultiplier (void) const
{ return m_autoThresholdMultiplier;
}

int CtConfiguration::artifact (void) const
{ return m_artifact;
}

double CtConfiguration::rawProfileGain (void) const
{ return m_rawProfileGain;
}

double CtConfiguration::rawProfileOffset (void) const
{ return m_rawProfileOffset;
}

void CtConfiguration::setThreshold (const double threshold)
{ m_threshold = threshold;
}

void CtConfiguration::setAutoThreshold (const bool autoThreshold)
{ m_autoThreshold = autoThreshold;
}

void CtConfiguration::setAutoThresholdMultiplier (const double autoThresholdMultiplier)
{ m_autoThresholdMultiplier = autoThresholdMultiplier;
}

void CtConfiguration::setArtifact (const int artifact)
{ m_artifact = artifact;
}

void CtConfiguration::setRawProfileGain (const double rawProfileGain)
{ m_rawProfileGain = rawProfileGain;
}

void CtConfiguration::setRawProfileOffset (const double rawProfileOffset)
{ m_rawProfileOffset = rawProfileOffset;
}

QJsonObject CtConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("threshold", m_threshold);
  object.insert ("autoThreshold", m_autoThreshold);
  object.insert ("autoThresholdMultiplier", m_autoThresholdMultiplier);
  object.insert ("artifact", m_artifact);
  object.insert ("rawProfileGain", m_rawProfileGain);
  object.insert ("rawProfileOffset", m_rawProfileOffset);
  return object;
}
