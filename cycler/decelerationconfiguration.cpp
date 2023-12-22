#include "decelerationconfiguration.h"

const bool DecelerationConfiguration::defaultEnable = true;
const int DecelerationConfiguration::defaultStepSize = 150;
const double DecelerationConfiguration::defaultInterval = 0.5;

DecelerationConfiguration::DecelerationConfiguration (void) :
  m_enable (defaultEnable),
  m_stepSize (defaultStepSize),
  m_interval (defaultInterval)
{
}

DecelerationConfiguration::DecelerationConfiguration (const bool enable, const int stepSize, const double interval) :
  m_enable (enable),
  m_stepSize (stepSize),
  m_interval (interval)
{
}

DecelerationConfiguration::DecelerationConfiguration (const QJsonObject &object) :
  m_enable (object.value ("enable").toBool (defaultEnable)),
  m_stepSize (object.value ("stepSize").toInt (defaultStepSize)),
  m_interval (object.value ("interval").toDouble (defaultInterval))
{
}

bool DecelerationConfiguration::enable (void) const
{ return m_enable;
}

int DecelerationConfiguration::stepSize (void) const
{ return m_stepSize;
}

double DecelerationConfiguration::interval (void) const
{ return m_interval;
}

void DecelerationConfiguration::setEnable (const bool enable)
{ m_enable = enable;
}

void DecelerationConfiguration::setStepSize (const int stepSize)
{ m_stepSize = stepSize;
}

void DecelerationConfiguration::setInterval (const double interval)
{ m_interval = interval;
}

QJsonObject DecelerationConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("enable", m_enable);
  object.insert ("stepSize", m_stepSize);
  object.insert ("interval", m_interval);
  return object;
}
