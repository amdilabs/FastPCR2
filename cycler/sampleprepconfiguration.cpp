#include "sampleprepconfiguration.h"

const bool SamplePrepConfiguration::defaultEnable = false;
const int SamplePrepConfiguration::defaultPwmFreq = 100000;
const double SamplePrepConfiguration::defaultHeatingDuration = 15.0;

SamplePrepConfiguration::SamplePrepConfiguration (void) :
  m_enable (defaultEnable),
  m_pwmFreq (defaultPwmFreq),
  m_heatingDuration (defaultHeatingDuration)
{
}

SamplePrepConfiguration::SamplePrepConfiguration (const bool enable, const int pwmFreq, const double heatingDuration) :
  m_enable (enable),
  m_pwmFreq (pwmFreq),
  m_heatingDuration (heatingDuration)
{
}

SamplePrepConfiguration::SamplePrepConfiguration (const QJsonObject &object) :
  m_enable (object.value ("enable").toBool (defaultEnable)),
  m_pwmFreq (object.value ("pwmFreq").toInt (defaultPwmFreq)),
  m_heatingDuration (object.value ("heatingDuration").toDouble (defaultHeatingDuration))
{
}

bool SamplePrepConfiguration::enable (void) const
{ return m_enable;
}

int SamplePrepConfiguration::pwmFreq (void) const
{ return m_pwmFreq;
}

double SamplePrepConfiguration::heatingDuration (void) const
{ return m_heatingDuration;
}

void SamplePrepConfiguration::setEnable (const bool enable)
{ m_enable = enable;
}

void SamplePrepConfiguration::setPwmFreq (const int pwmFreq)
{ m_pwmFreq = pwmFreq;
}

void SamplePrepConfiguration::setHeatingDuration (const double heatingDuration)
{ m_heatingDuration = heatingDuration;
}

QJsonObject SamplePrepConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("enable", m_enable);
  object.insert ("pwmFreq", m_pwmFreq);
  object.insert ("heatingDuration", m_heatingDuration);
  return object;
}
