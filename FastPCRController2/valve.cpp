#include "valve.h"

const double Valve::defaultAngle = 0.0;
const double Valve::defaultRadius = 0.0;
const double Valve::defaultCurrent = 0.0;
const double Valve::defaultDuration = 1.0;

Valve::Valve (void) :
  m_angle (defaultAngle),
  m_radius (defaultRadius),
  m_current (defaultCurrent),
  m_duration (defaultDuration)
{
}

Valve::Valve (const double angle, const double radius, const double current, const double duration) :
  m_angle (angle),
  m_radius (radius),
  m_current (current),
  m_duration (duration)
{
}

Valve::Valve (const QJsonObject &object) :
  m_angle (object.value ("angle").toDouble (defaultAngle)),
  m_radius (object.value ("radius").toDouble (defaultRadius)),
  m_current (object.value ("current").toDouble (defaultCurrent)),
  m_duration (object.value ("duration").toDouble (defaultDuration))
{
}

double Valve::angle (void) const
{ return m_angle;
}

double Valve::radius (void) const
{ return m_radius;
}

double Valve::current (void) const
{ return m_current;
}

double Valve::duration (void) const
{ return m_duration;
}

void Valve::setAngle (const double angle)
{ m_angle = angle;
}

void Valve::setRadius (const double radius)
{ m_radius = radius;
}

void Valve::setCurrent (const double current)
{ m_current = current;
}

void Valve::setDuration (const double duration)
{ m_duration = duration;
}

QJsonObject Valve::toJson (void) const
{ QJsonObject object;
  object.insert ("angle", m_angle);
  object.insert ("radius", m_radius);
  object.insert ("current", m_current);
  object.insert ("duration", m_duration);
  return object;
}

int Valve::angleTicks (const double ticksPerRevolution) const
{ return qRound (m_angle * (ticksPerRevolution / 360.0));
}

int Valve::radiusSteps (const double stepsPerMm) const
{ return qRound (m_radius * stepsPerMm);
}
