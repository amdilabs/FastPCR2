#include "meteringconfiguration.h"

const double MeteringConfiguration::defaultEvacuationVelocity = 3000.0;
const double MeteringConfiguration::defaultEvacuationTime = 10.0;
const double MeteringConfiguration::defaultMeteringVelocity = 1000.0;
const double MeteringConfiguration::defaultMeteringTime = 5.0;
const double MeteringConfiguration::defaultAcceleration = 8000.0;
const double MeteringConfiguration::defaultDeceleration = 8000.0;

MeteringConfiguration::MeteringConfiguration (void) :
  m_evacuationVelocity (defaultEvacuationVelocity),
  m_evacuationTime (defaultEvacuationTime),
  m_meteringVelocity (defaultMeteringVelocity),
  m_meteringTime (defaultMeteringTime),
  m_acceleration (defaultAcceleration),
  m_deceleration (defaultDeceleration)
{
}

MeteringConfiguration::MeteringConfiguration (const double evacuationVelocity, const double evacuationTime, const double meteringVelocity, const double meteringTime, const double acceleration, const double deceleration) :
  m_evacuationVelocity (evacuationVelocity),
  m_evacuationTime (evacuationTime),
  m_meteringVelocity (meteringVelocity),
  m_meteringTime (meteringTime),
  m_acceleration (acceleration),
  m_deceleration (deceleration)
{
}

MeteringConfiguration::MeteringConfiguration (const QJsonObject &object) :
  m_evacuationVelocity (object.value ("evacuationVelocity").toDouble (defaultEvacuationVelocity)),
  m_evacuationTime (object.value ("evacuationTime").toDouble (defaultEvacuationTime)),
  m_meteringVelocity (object.value ("meteringVelocity").toDouble (defaultMeteringVelocity)),
  m_meteringTime (object.value ("meteringTime").toDouble (defaultMeteringTime)),
  m_acceleration (object.value ("acceleration").toDouble (defaultAcceleration)),
  m_deceleration (object.value ("deceleration").toDouble (defaultDeceleration))
{
}

double MeteringConfiguration::evacuationVelocity (void) const
{ return m_evacuationVelocity;
}

double MeteringConfiguration::evacuationTime (void) const
{ return m_evacuationTime;
}

double MeteringConfiguration::meteringVelocity (void) const
{ return m_meteringVelocity;
}

double MeteringConfiguration::meteringTime (void) const
{ return m_meteringTime;
}

double MeteringConfiguration::acceleration (void) const
{ return m_acceleration;
}

double MeteringConfiguration::deceleration (void) const
{ return m_deceleration;
}

void MeteringConfiguration::setEvacuationVelocity (const double evacuationVelocity)
{ m_evacuationVelocity = evacuationVelocity;
}

void MeteringConfiguration::setEvacuationTime (const double evacuationTime)
{ m_evacuationTime = evacuationTime;
}

void MeteringConfiguration::setMeteringVelocity (const double meteringVelocity)
{ m_meteringVelocity = meteringVelocity;
}

void MeteringConfiguration::setMeteringTime (const double meteringTime)
{ m_meteringTime = meteringTime;
}

void MeteringConfiguration::setAcceleration (const double acceleration)
{ m_acceleration = acceleration;
}

void MeteringConfiguration::setDeceleration (const double deceleration)
{ m_deceleration = deceleration;
}

QJsonObject MeteringConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("evacuationVelocity", m_evacuationVelocity);
  object.insert ("evacuationTime", m_evacuationTime);
  object.insert ("meteringVelocity", m_meteringVelocity);
  object.insert ("meteringTime", m_meteringTime);
  object.insert ("acceleration", m_acceleration);
  object.insert ("deceleration", m_deceleration);
  return object;
}
