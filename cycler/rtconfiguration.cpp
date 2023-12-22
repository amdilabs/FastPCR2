#include "rtconfiguration.h"

const bool RtConfiguration::defaultRtEnable = false;
const double RtConfiguration::defaultRtTemperature = 64.0;
const double RtConfiguration::defaultRtTime = 60.0;
const double RtConfiguration::defaultActivationTemperature = 98.0;
const double RtConfiguration::defaultActivationTime = 30.0;

RtConfiguration::RtConfiguration (void) :
  m_rtEnable (defaultRtEnable),
  m_rtTemperature (defaultRtTemperature),
  m_rtTime (defaultRtTime),
  m_activationTemperature (defaultActivationTemperature),
  m_activationTime (defaultActivationTime)
{
}

RtConfiguration::RtConfiguration (const bool rtEnable, const double rtTemperature, const double rtTime, const double activationTemperature, const double activationTime) :
  m_rtEnable (rtEnable),
  m_rtTemperature (rtTemperature),
  m_rtTime (rtTime),
  m_activationTemperature (activationTemperature),
  m_activationTime (activationTime)
{
}

RtConfiguration::RtConfiguration (const QJsonObject &object) :
  m_rtEnable (object.value ("rtEnable").toBool (defaultRtEnable)),
  m_rtTemperature (object.value ("rtTemperature").toDouble (defaultRtTemperature)),
  m_rtTime (object.value ("rtTime").toDouble (defaultRtTime)),
  m_activationTemperature (object.value ("activationTemperature").toDouble (defaultActivationTemperature)),
  m_activationTime (object.value ("activationTime").toDouble (defaultActivationTime))
{
}

bool RtConfiguration::rtEnable (void) const
{ return m_rtEnable;
}

double RtConfiguration::rtTemperature (void) const
{ return m_rtTemperature;
}

double RtConfiguration::rtTime (void) const
{ return m_rtTime;
}

double RtConfiguration::activationTemperature (void) const
{ return m_activationTemperature;
}

double RtConfiguration::activationTime (void) const
{ return m_activationTime;
}

void RtConfiguration::setRtEnable (const bool rtEnable)
{ m_rtEnable = rtEnable;
}

void RtConfiguration::setRtTemperature (const double rtTemperature)
{ m_rtTemperature = rtTemperature;
}

void RtConfiguration::setRtTime (const double rtTime)
{ m_rtTime = rtTime;
}

void RtConfiguration::setActivationTemperature (const double activationTemperature)
{ m_activationTemperature = activationTemperature;
}

void RtConfiguration::setActivationTime (const double activationTime)
{ m_activationTime = activationTime;
}

QJsonObject RtConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("rtEnable", m_rtEnable);
  object.insert ("rtTemperature", m_rtTemperature);
  object.insert ("rtTime", m_rtTime);
  object.insert ("activationTemperature", m_activationTemperature);
  object.insert ("activationTime", m_activationTime);
  return object;
}
