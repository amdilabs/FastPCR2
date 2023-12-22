#include "diskconfiguration.h"

const double DiskConfiguration::defaultCuvetteOffset = 288.0;
const double DiskConfiguration::defaultCuvetteSeparation = -13.5;
const double DiskConfiguration::defaultHeater65Location = 168.0;
const double DiskConfiguration::defaultHeater95Location = 309.0;
const Valve DiskConfiguration::defaultHyperbaricValve = Valve (10.0, 0.0, 0.0, 1.0);
const Valve DiskConfiguration::defaultMeteringValve = Valve (20.0, 0.0, 0.0, 1.0);
const Valve DiskConfiguration::defaultInsertValve = Valve (30.0, 0.0, 0.0, 1.0);

DiskConfiguration::DiskConfiguration (void) :
  m_cuvetteOffset (defaultCuvetteOffset),
  m_cuvetteSeparation (defaultCuvetteSeparation),
  m_heater65Location (defaultHeater65Location),
  m_heater95Location (defaultHeater95Location),
  m_hyperbaricValve (defaultHyperbaricValve),
  m_meteringValve (defaultMeteringValve),
  m_insertValve (defaultInsertValve)
{
}

DiskConfiguration::DiskConfiguration (const double cuvetteOffset, const double cuvetteSeparation, const double heater65Location, const double heater95Location, const Valve &hyperbaricValve, const Valve &meteringValve, const Valve &insertValve) :
  m_cuvetteOffset (cuvetteOffset),
  m_cuvetteSeparation (cuvetteSeparation),
  m_heater65Location (heater65Location),
  m_heater95Location (heater95Location),
  m_hyperbaricValve (hyperbaricValve),
  m_meteringValve (meteringValve),
  m_insertValve (insertValve)
{
}

DiskConfiguration::DiskConfiguration (const QJsonObject &object) :
  m_cuvetteOffset (object.value ("cuvetteOffset").toDouble (defaultCuvetteOffset)),
  m_cuvetteSeparation (object.value ("cuvetteSeparation").toDouble (defaultCuvetteSeparation)),
  m_heater65Location (object.value ("heater65Location").toDouble (defaultHeater65Location)),
  m_heater95Location (object.value ("heater95Location").toDouble (defaultHeater95Location)),
  m_hyperbaricValve (Valve (object.value ("hyperbaricValve").toObject (defaultHyperbaricValve.toJson ()))),
  m_meteringValve (Valve (object.value ("meteringValve").toObject (defaultMeteringValve.toJson ()))),
  m_insertValve (Valve (object.value ("insertValve").toObject (defaultInsertValve.toJson ())))
{
}

double DiskConfiguration::cuvetteOffset (void) const
{ return m_cuvetteOffset;
}

double DiskConfiguration::cuvetteSeparation (void) const
{ return m_cuvetteSeparation;
}

double DiskConfiguration::heater65Location (void) const
{ return m_heater65Location;
}

double DiskConfiguration::heater95Location (void) const
{ return m_heater95Location;
}

Valve DiskConfiguration::hyperbaricValve (void) const
{ return m_hyperbaricValve;
}

Valve DiskConfiguration::meteringValve (void) const
{ return m_meteringValve;
}

Valve DiskConfiguration::insertValve (void) const
{ return m_insertValve;
}

void DiskConfiguration::setCuvetteOffset (const double cuvetteOffset)
{ m_cuvetteOffset = cuvetteOffset;
}

void DiskConfiguration::setCuvetteSeparation (const double cuvetteSeparation)
{ m_cuvetteSeparation = cuvetteSeparation;
}

void DiskConfiguration::setHeater65Location (const double heater65Location)
{ m_heater65Location = heater65Location;
}

void DiskConfiguration::setHeater95Location (const double heater95Location)
{ m_heater95Location = heater95Location;
}

void DiskConfiguration::setHyperbaricValve (const Valve &hyperbaricValve)
{ m_hyperbaricValve = hyperbaricValve;
}

void DiskConfiguration::setMeteringValve (const Valve &meteringValve)
{ m_meteringValve = meteringValve;
}

void DiskConfiguration::setInsertValve (const Valve &insertValve)
{ m_insertValve = insertValve;
}

int DiskConfiguration::cuvetteOffsetTicks (const double ticksPerRevolution) const
{ return ticks (m_cuvetteOffset, ticksPerRevolution);
}

int DiskConfiguration::cuvetteSeparationTicks (const double ticksPerRevolution) const
{ return ticks (m_cuvetteSeparation, ticksPerRevolution);
}

int DiskConfiguration::heater65LocationTicks (const double ticksPerRevolution) const
{ return ticks (m_heater65Location, ticksPerRevolution);
}

int DiskConfiguration::heater95LocationTicks (const double ticksPerRevolution) const
{ return ticks (m_heater95Location, ticksPerRevolution);
}

double DiskConfiguration::cuvetteLocation (const int cuvette) const
{ return m_cuvetteOffset + (static_cast<double> (cuvette) * m_cuvetteSeparation);
}

int DiskConfiguration::cuvetteLocationTicks (const int cuvette, const double ticksPerRevolution) const
{ return ticks (cuvetteLocation (cuvette), ticksPerRevolution);
}

QJsonObject DiskConfiguration::toJson (void) const
{ QJsonObject object;
  object.insert ("cuvetteOffset", m_cuvetteOffset);
  object.insert ("cuvetteSeparation", m_cuvetteSeparation);
  object.insert ("heater65Location", m_heater65Location);
  object.insert ("heater95Location", m_heater95Location);
  object.insert ("hyperbaricValve", m_hyperbaricValve.toJson ());
  object.insert ("meteringValve", m_meteringValve.toJson ());
  object.insert ("insertValve", m_insertValve.toJson ());
  return object;
}

int DiskConfiguration::ticks (const double degrees, const double ticksPerRevolution)
{ return qRound (degrees * (ticksPerRevolution / 360.0));
}
