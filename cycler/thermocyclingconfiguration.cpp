#include "thermocyclingconfiguration.h"

const int ThermocyclingConfiguration::defaultNumOfCycles = 50;
const double ThermocyclingConfiguration::defaultDenaturationTemperature = 98.0;
const double ThermocyclingConfiguration::defaultDenaturationTime = 2.0;
const double ThermocyclingConfiguration::defaultAnnealingTemperature = 65.0;
const double ThermocyclingConfiguration::defaultAnnealingTime = 5.0;

ThermocyclingConfiguration::ThermocyclingConfiguration (void) :
  m_numOfCycles (defaultNumOfCycles),
  m_denaturationTemperature (defaultDenaturationTemperature),
  m_denaturationTime (defaultDenaturationTime),
  m_annealingTemperature (defaultAnnealingTemperature),
  m_annealingTime (defaultAnnealingTime)
{
}

ThermocyclingConfiguration::ThermocyclingConfiguration (const int numOfCycles, const double denaturationTemperature, const double denaturationTime, const double annealingTemperature, const double annealingTime) :
  m_numOfCycles (numOfCycles),
  m_denaturationTemperature (denaturationTemperature),
  m_denaturationTime (denaturationTime),
  m_annealingTemperature (annealingTemperature),
  m_annealingTime (annealingTime)
{
}

ThermocyclingConfiguration::ThermocyclingConfiguration (const QJsonObject &object) :
  m_numOfCycles (object.value ("numOfCycles").toInt (defaultNumOfCycles)),
  m_denaturationTemperature (object.value ("denaturationTemperature").toDouble (defaultDenaturationTemperature)),
  m_denaturationTime (object.value ("denaturationTime").toDouble (defaultDenaturationTime)),
  m_annealingTemperature (object.value ("annealingTemperature").toDouble (defaultAnnealingTemperature)),
  m_annealingTime (object.value ("annealingTime").toDouble (defaultAnnealingTime))
{
}

int ThermocyclingConfiguration::numOfCycles (void) const
{ return m_numOfCycles;
}

double ThermocyclingConfiguration::denaturationTemperature (void) const
{ return m_denaturationTemperature;
}

double ThermocyclingConfiguration::denaturationTime (void) const
{ return m_denaturationTime;
}

double ThermocyclingConfiguration::annealingTemperature (void) const
{ return m_annealingTemperature;
}

double ThermocyclingConfiguration::annealingTime (void) const
{ return m_annealingTime;
}

void ThermocyclingConfiguration::setNumOfCycles (const int numOfCycles)
{ m_numOfCycles = numOfCycles;
}

void ThermocyclingConfiguration::setDenaturationTemperature (const double denaturationTemperature)
{ m_denaturationTemperature = denaturationTemperature;
}

void ThermocyclingConfiguration::setDenaturationTime (const double denaturationTime)
{ m_denaturationTime = denaturationTime;
}

void ThermocyclingConfiguration::setAnnealingTemperature (const double annealingTemperature)
{ m_annealingTemperature = annealingTemperature;
}

void ThermocyclingConfiguration::setAnnealingTime (const double annealingTime)
{ m_annealingTime = annealingTime;
}

QJsonObject ThermocyclingConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("numOfCycles", m_numOfCycles);
  object.insert ("denaturationTemperature", m_denaturationTemperature);
  object.insert ("denaturationTime", m_denaturationTime);
  object.insert ("annealingTemperature", m_annealingTemperature);
  object.insert ("annealingTime", m_annealingTime);
  return object;
}
