#ifndef PCRPROTOCOL_H
#define PCRPROTOCOL_H

#include "prehbhconfiguration.h"
#include "fillingconfiguration.h"
#include "meteringconfiguration.h"
#include "resuspensionconfiguration.h"
#include "rtconfiguration.h"
#include "sampleprepconfiguration.h"
#include "thermocyclingconfiguration.h"

class PcrProtocol
{ public:
    PcrProtocol (void);
    PcrProtocol (const PreHbhConfiguration &preHbhConfig, const SamplePrepConfiguration &samplePrepConfig, const MeteringConfiguration &meteringConfig, const ResuspensionConfiguration &resuspensionConfig, const FillingConfiguration &fillingConfig, const RtConfiguration &rtConfig, const ThermocyclingConfiguration &thermocyclingConfig);
    PcrProtocol (const QJsonObject &object);

    PreHbhConfiguration preHbhConfig (void) const;
    SamplePrepConfiguration samplePrepConfig (void) const;
    MeteringConfiguration meteringConfig (void) const;
    ResuspensionConfiguration resuspensionConfig (void) const;
    FillingConfiguration fillingConfig (void) const;
    RtConfiguration rtConfig (void) const;
    ThermocyclingConfiguration thermocyclingConfig (void) const;

    void setPreHbhConfig (const PreHbhConfiguration &preHbhConfig);
    void setSamplePrepConfig (const SamplePrepConfiguration &samplePrepConfig);
    void setMeteringConfig (const MeteringConfiguration &meteringConfig);
    void setResuspensionConfig (const ResuspensionConfiguration &resuspensionConfig);
    void setFillingConfig (const FillingConfiguration &fillingConfig);
    void setRtConfig (const RtConfiguration &rtConfig);
    void setThermocyclingConfig (const ThermocyclingConfiguration &thermocyclingConfig);
    void setSealerEnable(const int sealerMask);

    QJsonObject toJson (void) const;

  private:
    PreHbhConfiguration m_preHbhConfig;
    SamplePrepConfiguration m_samplePrepConfig;
    MeteringConfiguration m_meteringConfig;
    ResuspensionConfiguration m_resuspensionConfig;
    FillingConfiguration m_fillingConfig;
    RtConfiguration m_rtConfig;
    ThermocyclingConfiguration m_thermocyclingConfig;
};

#endif // PCRPROTOCOL_H
