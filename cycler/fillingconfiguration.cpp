#include "fillingconfiguration.h"

const double FillingConfiguration::defaultVelocity = 3000.0;
const double FillingConfiguration::defaultTime = 20.0;
const double FillingConfiguration::defaultAcceleration = 8000.0;
const double FillingConfiguration::defaultDeceleration = 8000.0;
const double FillingConfiguration::defaultSealerTemperature = 230.0;
const double FillingConfiguration::defaultSealingTime = 2.0;
const int FillingConfiguration::defaultSealerMask = 0x1FF;   // Location 0 for waste, 1 to 8 for cuvettes
const bool FillingConfiguration::defaultReverse = false;


FillingConfiguration::FillingConfiguration (void) :
  m_velocity (defaultVelocity),
  m_time (defaultTime),
  m_acceleration (defaultAcceleration),
  m_deceleration (defaultDeceleration),
  m_sealerTemperature (defaultSealerTemperature),
  m_sealingTime (defaultSealingTime),
  m_sealerMask (defaultSealerMask),
  m_reverse (defaultReverse)
{
}

FillingConfiguration::FillingConfiguration (const double velocity, const double time, const double acceleration, const double deceleration, const double sealerTemperature, const double sealingTime, const bool reverse) :
  m_velocity (velocity),
  m_time (time),
  m_acceleration (acceleration),
  m_deceleration (deceleration),
  m_sealerTemperature (sealerTemperature),
  m_sealingTime (sealingTime),
  m_reverse (reverse)
{
}

FillingConfiguration::FillingConfiguration (const QJsonObject &object) :
  m_velocity (object.value ("velocity").toDouble (defaultVelocity)),
  m_time (object.value ("time").toDouble (defaultTime)),
  m_acceleration (object.value ("acceleration").toDouble (defaultAcceleration)),
  m_deceleration (object.value ("deceleration").toDouble (defaultDeceleration)),
  m_sealerTemperature (object.value ("sealerTemperature").toDouble (defaultSealerTemperature)),
  m_sealingTime (object.value ("sealingTime").toDouble (defaultSealingTime)),
  m_reverse (object.value ("reverse").toBool (defaultReverse))
{
}

double FillingConfiguration::velocity (void) const
{ return m_velocity;
}

double FillingConfiguration::time (void) const
{ return m_time;
}

double FillingConfiguration::acceleration (void) const
{ return m_acceleration;
}

double FillingConfiguration::deceleration (void) const
{ return m_deceleration;
}

double FillingConfiguration::sealerTemperature (void) const
{ return m_sealerTemperature;
}

double FillingConfiguration::sealingTime (void) const
{ return m_sealingTime;
}

bool FillingConfiguration::reverse (void) const
{ return m_reverse;
}

void FillingConfiguration::setVelocity (const double velocity)
{ m_velocity = velocity;
}

void FillingConfiguration::setTime (const double time)
{ m_time = time;
}

void FillingConfiguration::setAcceleration (const double acceleration)
{ m_acceleration = acceleration;
}

void FillingConfiguration::setDeceleration (const double deceleration)
{ m_deceleration = deceleration;
}

void FillingConfiguration::setSealerTemperature (const double sealerTemperature)
{ m_sealerTemperature = sealerTemperature;
}

void FillingConfiguration::setSealingTime (const double sealingTime)
{ m_sealingTime = sealingTime;
}

void FillingConfiguration::setSealerEnable (const int sealerMask)
{ m_sealerMask = sealerMask;
}

int FillingConfiguration::sealerEnable (void) const
{ return m_sealerMask;
}

void FillingConfiguration::setReverse (const bool reverse)
{ m_reverse = reverse;
}

QJsonObject FillingConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("velocity", m_velocity);
  object.insert ("time", m_time);
  object.insert ("acceleration", m_acceleration);
  object.insert ("deceleration", m_deceleration);
  object.insert ("sealerTemperature", m_sealerTemperature);
  object.insert ("sealingTime", m_sealingTime);
  object.insert ("reverse", m_reverse);
  return object;
}
