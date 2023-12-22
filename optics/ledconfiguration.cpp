#include "ledconfiguration.h"

const double LedConfiguration::defaultCurrent = 0.5;
const double LedConfiguration::defaultShunt = 0.1;

LedConfiguration::LedConfiguration (void) :
  m_current (defaultCurrent),
  m_shunt (defaultShunt)
{
}

LedConfiguration::LedConfiguration (const double current, const double shunt) :
  m_current (current),
  m_shunt (shunt)
{
}

LedConfiguration::LedConfiguration (const QJsonObject &object) :
  m_current (object.value ("current").toDouble (defaultCurrent)),
  m_shunt (object.value ("shunt").toDouble (defaultShunt))
{
}

double LedConfiguration::current (void) const
{ return m_current;
}

double LedConfiguration::shunt (void) const
{ return m_shunt;
}

void LedConfiguration::setCurrent (const double current)
{ m_current = current;
}

void LedConfiguration::setShunt (const double shunt)
{ m_shunt = shunt;
}

double LedConfiguration::vSense (void) const
{ return m_current * m_shunt;
}

QJsonObject LedConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("current", m_current);
  object.insert ("shunt", m_shunt);
  return object;
}
