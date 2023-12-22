#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../cycler/prehbhconfiguration.h"
#include "../cycler/prehbhmixingconfiguration.h"
#include "../cycler/fillingconfiguration.h"
#include "../cycler/interface.h"
#include "../cycler/meteringconfiguration.h"
#include "../cycler/mixingconfiguration.h"
#include "../cycler/rtconfiguration.h"
#include "../cycler/resuspensionconfiguration.h"
#include "../cycler/sampleprepconfiguration.h"
#include "../cycler/thermocyclingconfiguration.h"
#include "diskconfiguration.h"
#include "progresstracker.h"

class Scheduler
{ public:
    static bool stepsPreHbh (Interface *interface, const PreHbhConfiguration &preHbhConfig, ProgressTracker *tracker);
    static bool stepsPreTest (Interface *interface, const FillingConfiguration &fillingConfig, const RtConfiguration &rtConfig, const ThermocyclingConfiguration &thermocyclingConfig, ProgressTracker *tracker);
    static bool stepsSamplePrep (Interface *interface, const DiskConfiguration &diskConfig, const SamplePrepConfiguration &samplePrepConfig, ProgressTracker *tracker);
    static bool stepsMetering (Interface *interface, const DiskConfiguration &diskConfig, const SamplePrepConfiguration &samplePrepConfig, const MeteringConfiguration &meteringConfig, ProgressTracker *tracker);
    static bool stepsResuspension (Interface *interface, const DiskConfiguration &diskConfig, const SamplePrepConfiguration &samplePrepConfig, const ResuspensionConfiguration &resuspensionConfig, ProgressTracker *tracker);
    static bool stepsFilling (Interface *interface, const DiskConfiguration &diskConfig, const FillingConfiguration &fillingConfig, const double standbyTemperatureSealer, ProgressTracker *tracker);
    static bool stepsPcrPause (Interface *interface, ProgressTracker *tracker);
    static bool stepsPreHbhPause (Interface *interface, ProgressTracker *tracker);
    static bool stepsRt (Interface *interface, const DiskConfiguration &diskConfig, const RtConfiguration &rtConfig, const ThermocyclingConfiguration &thermocyclingConfig, ProgressTracker *tracker);
    static bool stepsThermocycling (Interface *interface, const DiskConfiguration &diskConfig, const ThermocyclingConfiguration &config, ProgressTracker *tracker);
    static bool stepsPostTest (Interface *interface, const double standbyTemperature65, const double standbyTemperature95, ProgressTracker *tracker);

  private:
    static const double defaultAcceleration;
    static const double defaultDeceleration;

    static bool stepsSealingForward (Interface *interface, const DiskConfiguration &diskConfig, const FillingConfiguration &config, ProgressTracker *tracker);
    static bool stepsSealingReverse (Interface *interface, const DiskConfiguration &diskConfig, const FillingConfiguration &config, ProgressTracker *tracker);
    static bool stepsSpindleHome (Interface *interface, const Progress::State state, ProgressTracker *tracker);
    static bool stepsSpinStart (Interface *interface, const int velocity, const double time, ProgressTracker *tracker);
    static bool stepsSpinStop (Interface *interface, ProgressTracker *tracker);
    static bool stepsPosition (Interface *interface, const int position, const bool direction, ProgressTracker *tracker);
    static bool stepsBurnValve (Interface *interface, const Progress::State state, const Valve &valve, ProgressTracker *tracker);
    static bool stepsBurnValve_Ming1 (Interface *interface, const Progress::State state, const Valve &valve, ProgressTracker *tracker);
    static bool stepsBurnValve_Ming2 (Interface *interface, const Progress::State state, const Valve &valve, ProgressTracker *tracker);
    static bool stepsBurnValve_skipHoming (Interface *interface, const Progress::State state, const Valve &valve, ProgressTracker *tracker);
    static bool stepsPreHbhMixing (Interface *interface, const PreHbhMixingConfiguration &preHbHMixingConfig, ProgressTracker *tracker);
    static bool stepsResuspensionMixing (Interface *interface, const MixingConfiguration &mixingConfig, ProgressTracker *tracker);
};

#endif // SCHEDULER_H
