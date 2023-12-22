#include "prehbhmixingconfiguration.h"

const int PreHbhMixingConfiguration::defaultNumOfCycles = 10;
const double PreHbhMixingConfiguration::defaultInterval = 1.0;
const double PreHbhMixingConfiguration::defaultVelocity = 1000.0;
const double PreHbhMixingConfiguration::defaultAcceleration = 8000.0;
const double PreHbhMixingConfiguration::defaultDeceleration = 8000.0;

PreHbhMixingConfiguration::PreHbhMixingConfiguration (void) :
  m_numOfCycles (defaultNumOfCycles),
  m_interval (defaultInterval),
  m_velocity (defaultVelocity),
  m_acceleration (defaultAcceleration),
  m_deceleration (defaultDeceleration)
{
}

PreHbhMixingConfiguration::PreHbhMixingConfiguration (const int numOfCycles, const double interval, const double velocity, const double acceleration, const double deceleration) :
  m_numOfCycles (numOfCycles),
  m_interval (interval),
  m_velocity (velocity),
  m_acceleration (acceleration),
  m_deceleration (deceleration)
{
}

PreHbhMixingConfiguration::PreHbhMixingConfiguration (const QJsonObject &object) :
  m_numOfCycles (object.value ("numOfCycles").toInt (defaultNumOfCycles)),
  m_interval (object.value ("interval").toDouble (defaultInterval)),
  m_velocity (object.value ("velocity").toDouble (defaultVelocity)),
  m_acceleration (object.value ("acceleration").toDouble (defaultAcceleration)),
  m_deceleration (object.value ("deceleration").toDouble (defaultDeceleration))
{
}

int PreHbhMixingConfiguration::numOfCycles (void) const
{ return m_numOfCycles;
}

double PreHbhMixingConfiguration::interval (void) const
{ return m_interval;
}

double PreHbhMixingConfiguration::velocity (void) const
{ return m_velocity;
}

double PreHbhMixingConfiguration::acceleration (void) const
{ return m_acceleration;
}

double PreHbhMixingConfiguration::deceleration (void) const
{ return m_deceleration;
}

void PreHbhMixingConfiguration::setNumOfCycles (const int numOfCycles)
{ m_numOfCycles = numOfCycles;
}

void PreHbhMixingConfiguration::setInterval (const double interval)
{ m_interval = interval;
}

void PreHbhMixingConfiguration::setVelocity (const double velocity)
{ m_velocity = velocity;
}

void PreHbhMixingConfiguration::setAcceleration (const double acceleration)
{ m_acceleration = acceleration;
}

void PreHbhMixingConfiguration::setDeceleration (const double deceleration)
{ m_deceleration = deceleration;
}

QJsonObject PreHbhMixingConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("numOfCycles", m_numOfCycles);
  object.insert ("interval", m_interval);
  object.insert ("velocity", m_velocity);
  object.insert ("acceleration", m_acceleration);
  object.insert ("deceleration", m_deceleration);
  return object;
}
