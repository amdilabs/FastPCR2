#include "resuspensionconfiguration.h"

const double ResuspensionConfiguration::defaultVelocity = 1000.0;
const double ResuspensionConfiguration::defaultTime = 5.0;
const double ResuspensionConfiguration::defaultAcceleration = 8000.0;
const double ResuspensionConfiguration::defaultDeceleration = 8000.0;

ResuspensionConfiguration::ResuspensionConfiguration (void) :
  m_velocity (defaultVelocity),
  m_time (defaultTime),
  m_acceleration (defaultAcceleration),
  m_deceleration (defaultDeceleration)
{
}

ResuspensionConfiguration::ResuspensionConfiguration (const double velocity, const double time, const double acceleration, const double deceleration, const MixingConfiguration &mixing) :
  m_velocity (velocity),
  m_time (time),
  m_acceleration (acceleration),
  m_deceleration (deceleration),
  m_mixing (mixing)
{
}

ResuspensionConfiguration::ResuspensionConfiguration (const QJsonObject &object) :
  m_velocity (object.value ("velocity").toDouble (defaultVelocity)),
  m_time (object.value ("time").toDouble (defaultTime)),
  m_acceleration (object.value ("acceleration").toDouble (defaultAcceleration)),
  m_deceleration (object.value ("deceleration").toDouble (defaultDeceleration)),
  m_mixing (MixingConfiguration (object.value ("mixing").toObject ()))
{
}

double ResuspensionConfiguration::velocity (void) const
{ return m_velocity;
}

double ResuspensionConfiguration::time (void) const
{ return m_time;
}

double ResuspensionConfiguration::acceleration (void) const
{ return m_acceleration;
}

double ResuspensionConfiguration::deceleration (void) const
{ return m_deceleration;
}

MixingConfiguration ResuspensionConfiguration::mixing (void) const
{ return m_mixing;
}

void ResuspensionConfiguration::setVelocity (const double velocity)
{ m_velocity = velocity;
}

void ResuspensionConfiguration::setTime (const double time)
{ m_time = time;
}

void ResuspensionConfiguration::setAcceleration (const double acceleration)
{ m_acceleration = acceleration;
}

void ResuspensionConfiguration::setDeceleration (const double deceleration)
{ m_deceleration = deceleration;
}

void ResuspensionConfiguration::setMixing (const MixingConfiguration &mixing)
{ m_mixing = mixing;
}

QJsonObject ResuspensionConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("velocity", m_velocity);
  object.insert ("time", m_time);
  object.insert ("acceleration", m_acceleration);
  object.insert ("deceleration", m_deceleration);
  object.insert ("mixing", m_mixing.toJson ());
  return object;
}
