#include "pcrprotocol.h"

PcrProtocol::PcrProtocol (void)
{
}

PcrProtocol::PcrProtocol (const PreHbhConfiguration &preHbhConfig, const SamplePrepConfiguration &samplePrepConfig, const MeteringConfiguration &meteringConfig, const ResuspensionConfiguration &resuspensionConfig, const FillingConfiguration &fillingConfig, const RtConfiguration &rtConfig, const ThermocyclingConfiguration &thermocyclingConfig) :
  m_preHbhConfig (preHbhConfig),
  m_samplePrepConfig (samplePrepConfig),
  m_meteringConfig (meteringConfig),
  m_resuspensionConfig (resuspensionConfig),
  m_fillingConfig (fillingConfig),
  m_rtConfig (rtConfig),
  m_thermocyclingConfig (thermocyclingConfig)
{
}

PcrProtocol::PcrProtocol (const QJsonObject &object) :
  m_preHbhConfig (PreHbhConfiguration (object.value ("preHbh").toObject ())),
  m_samplePrepConfig (SamplePrepConfiguration (object.value ("samplePrep").toObject ())),
  m_meteringConfig (MeteringConfiguration (object.value ("metering").toObject ())),
  m_resuspensionConfig (ResuspensionConfiguration (object.value ("resuspension").toObject ())),
  m_fillingConfig (FillingConfiguration (object.value ("filling").toObject ())),
  m_rtConfig (RtConfiguration (object.value ("rt").toObject ())),
  m_thermocyclingConfig (ThermocyclingConfiguration (object.value ("thermocycling").toObject ()))
{
}

PreHbhConfiguration PcrProtocol::preHbhConfig (void) const
{ return m_preHbhConfig;
}

SamplePrepConfiguration PcrProtocol::samplePrepConfig (void) const
{ return m_samplePrepConfig;
}

MeteringConfiguration PcrProtocol::meteringConfig (void) const
{ return m_meteringConfig;
}

ResuspensionConfiguration PcrProtocol::resuspensionConfig (void) const
{ return m_resuspensionConfig;
}

FillingConfiguration PcrProtocol::fillingConfig (void) const
{ return m_fillingConfig;
}

RtConfiguration PcrProtocol::rtConfig (void) const
{ return m_rtConfig;
}

ThermocyclingConfiguration PcrProtocol::thermocyclingConfig (void) const
{ return m_thermocyclingConfig;
}

void PcrProtocol::setPreHbhConfig (const PreHbhConfiguration &preHbhConfig)
{ m_preHbhConfig = preHbhConfig;
}

void PcrProtocol::setSamplePrepConfig (const SamplePrepConfiguration &samplePrepConfig)
{ m_samplePrepConfig = samplePrepConfig;
}

void PcrProtocol::setMeteringConfig (const MeteringConfiguration &meteringConfig)
{ m_meteringConfig = meteringConfig;
}

void PcrProtocol::setResuspensionConfig (const ResuspensionConfiguration &resuspensionConfig)
{ m_resuspensionConfig = resuspensionConfig;
}

void PcrProtocol::setFillingConfig (const FillingConfiguration &fillingConfig)
{ m_fillingConfig = fillingConfig;
}

void PcrProtocol::setRtConfig (const RtConfiguration &rtConfig)
{ m_rtConfig = rtConfig;
}

void PcrProtocol::setThermocyclingConfig (const ThermocyclingConfiguration &thermocyclingConfig)
{ m_thermocyclingConfig = thermocyclingConfig;
}

void PcrProtocol::setSealerEnable (const int sealerMask)
{ m_fillingConfig.setSealerEnable(sealerMask);
}

QJsonObject PcrProtocol::toJson (void) const
{ QJsonObject object;
  object.insert ("preHbh", m_preHbhConfig.toJson ());
  object.insert ("samplePrep", m_samplePrepConfig.toJson ());
  object.insert ("metering", m_meteringConfig.toJson ());
  object.insert ("resuspension", m_resuspensionConfig.toJson ());
  object.insert ("filling", m_fillingConfig.toJson ());
  object.insert ("rt", m_rtConfig.toJson ());
  object.insert ("thermocycling", m_thermocyclingConfig.toJson ());
  return object;
}
