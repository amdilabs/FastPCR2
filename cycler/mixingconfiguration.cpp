#include "mixingconfiguration.h"

const int MixingConfiguration::defaultNumOfCycles = 10;
const double MixingConfiguration::defaultInterval = 1.0;
const double MixingConfiguration::defaultVelocity = 1000.0;
const double MixingConfiguration::defaultAcceleration = 8000.0;
const double MixingConfiguration::defaultDeceleration = 8000.0;

MixingConfiguration::MixingConfiguration (void) :
  m_numOfCycles (defaultNumOfCycles),
  m_interval (defaultInterval),
  m_velocity (defaultVelocity),
  m_acceleration (defaultAcceleration),
  m_deceleration (defaultDeceleration)
{
}

MixingConfiguration::MixingConfiguration (const int numOfCycles, const double interval, const double velocity, const double acceleration, const double deceleration) :
  m_numOfCycles (numOfCycles),
  m_interval (interval),
  m_velocity (velocity),
  m_acceleration (acceleration),
  m_deceleration (deceleration)
{
}

MixingConfiguration::MixingConfiguration (const QJsonObject &object) :
  m_numOfCycles (object.value ("numOfCycles").toInt (defaultNumOfCycles)),
  m_interval (object.value ("interval").toDouble (defaultInterval)),
  m_velocity (object.value ("velocity").toDouble (defaultVelocity)),
  m_acceleration (object.value ("acceleration").toDouble (defaultAcceleration)),
  m_deceleration (object.value ("deceleration").toDouble (defaultDeceleration))
{
}

int MixingConfiguration::numOfCycles (void) const
{ return m_numOfCycles;
}

double MixingConfiguration::interval (void) const
{ return m_interval;
}

double MixingConfiguration::velocity (void) const
{ return m_velocity;
}

double MixingConfiguration::acceleration (void) const
{ return m_acceleration;
}

double MixingConfiguration::deceleration (void) const
{ return m_deceleration;
}

void MixingConfiguration::setNumOfCycles (const int numOfCycles)
{ m_numOfCycles = numOfCycles;
}

void MixingConfiguration::setInterval (const double interval)
{ m_interval = interval;
}

void MixingConfiguration::setVelocity (const double velocity)
{ m_velocity = velocity;
}

void MixingConfiguration::setAcceleration (const double acceleration)
{ m_acceleration = acceleration;
}

void MixingConfiguration::setDeceleration (const double deceleration)
{ m_deceleration = deceleration;
}

QJsonObject MixingConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("numOfCycles", m_numOfCycles);
  object.insert ("interval", m_interval);
  object.insert ("velocity", m_velocity);
  object.insert ("acceleration", m_acceleration);
  object.insert ("deceleration", m_deceleration);
  return object;
}
