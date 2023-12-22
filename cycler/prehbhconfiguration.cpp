#include "prehbhconfiguration.h"

const double PreHbhConfiguration::defaultVelocity = 1000.0;
const double PreHbhConfiguration::defaultTime = 5.0;
const double PreHbhConfiguration::defaultAcceleration = 8000.0;
const double PreHbhConfiguration::defaultDeceleration = 8000.0;

PreHbhConfiguration::PreHbhConfiguration (void) :
  m_velocity (defaultVelocity),
  m_time (defaultTime),
  m_acceleration (defaultAcceleration),
  m_deceleration (defaultDeceleration)
{
}

PreHbhConfiguration::PreHbhConfiguration (const double velocity, const double time, const double acceleration, const double deceleration, const PreHbhMixingConfiguration &mixing) :
  m_velocity (velocity),
  m_time (time),
  m_acceleration (acceleration),
  m_deceleration (deceleration),
  m_mixing (mixing)
{
}

PreHbhConfiguration::PreHbhConfiguration (const QJsonObject &object) :
  m_velocity (object.value ("velocity").toDouble (defaultVelocity)),
  m_time (object.value ("time").toDouble (defaultTime)),
  m_acceleration (object.value ("acceleration").toDouble (defaultAcceleration)),
  m_deceleration (object.value ("deceleration").toDouble (defaultDeceleration)),
  m_mixing (PreHbhMixingConfiguration (object.value ("mixing").toObject ()))
{
}

double PreHbhConfiguration::velocity (void) const
{ return m_velocity;
}

double PreHbhConfiguration::time (void) const
{ return m_time;
}

double PreHbhConfiguration::acceleration (void) const
{ return m_acceleration;
}

double PreHbhConfiguration::deceleration (void) const
{ return m_deceleration;
}

PreHbhMixingConfiguration PreHbhConfiguration::mixing (void) const
{ return m_mixing;
}

void PreHbhConfiguration::setVelocity (const double velocity)
{ m_velocity = velocity;
}

void PreHbhConfiguration::setTime (const double time)
{ m_time = time;
}

void PreHbhConfiguration::setAcceleration (const double acceleration)
{ m_acceleration = acceleration;
}

void PreHbhConfiguration::setDeceleration (const double deceleration)
{ m_deceleration = deceleration;
}

void PreHbhConfiguration::setMixing (const PreHbhMixingConfiguration &mixing)
{ m_mixing = mixing;
}

QJsonObject PreHbhConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("velocity", m_velocity);
  object.insert ("time", m_time);
  object.insert ("acceleration", m_acceleration);
  object.insert ("deceleration", m_deceleration);
  object.insert ("mixing", m_mixing.toJson ());
  return object;
}
