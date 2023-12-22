#include "cycler.h"
#include "scheduler.h"

const double Scheduler::defaultAcceleration = 8000.0;
const double Scheduler::defaultDeceleration = 8000.0;

bool Scheduler::stepsPreHbh (Interface *interface, const PreHbhConfiguration &preHbhConfig, ProgressTracker *tracker)
{ tracker->mark (Progress (Progress::PreHbhSpinAndMixing, Progress::None, 0), 2);

  if (!interface->schSpindleSetAccelDecel (preHbhConfig.acceleration (), preHbhConfig.deceleration ())) return false;
  if (!stepsSpinStart (interface, preHbhConfig.velocity (), preHbhConfig.time (), tracker)) return false;
  if (!stepsSpinStop (interface, tracker)) return false;
  if (!interface->schSpindleSetAccelDecel (defaultAcceleration, defaultDeceleration)) return false;

  if (!stepsPreHbhMixing (interface, preHbhConfig.mixing (), tracker)) return false;

  if (!stepsSpinStop (interface, tracker)) return false;

  return true;
}

bool Scheduler::stepsPreTest (Interface *interface, const FillingConfiguration &fillingConfig, const RtConfiguration &rtConfig, const ThermocyclingConfiguration &thermocyclingConfig, ProgressTracker *tracker)
{ tracker->mark (Progress (Progress::PreTest, Progress::None, 0), 11);

  if (!interface->schGripperAllToPos (Interface::FullOpen)) return false;
  if (!interface->schGripperWaitPosition65c ()) return false;
  if (!interface->schGripperWaitPosition95c ()) return false;
  if (!interface->schGripperWaitPositionSealer ()) return false;

  if (!interface->schHeaterOn65c ((rtConfig.rtEnable ()?rtConfig.rtTemperature ():thermocyclingConfig.annealingTemperature ()), Cycler::pcrHeaterRate)) return false;
  if (!interface->schHeaterOn95c (rtConfig.activationTemperature (), Cycler::pcrHeaterRate)) return false;
  if (!interface->schHeaterOnSealer (fillingConfig.sealerTemperature (), Cycler::heatSealerRate)) return false;

  if (!interface->schStepperHome (true)) return false;
  if (!interface->schStepperWaitHome ()) return false;
  if (!interface->schTimerStart1 ()) return false;
  if (!interface->schTimerWait1 (500)) return false;

  return true;
}

bool Scheduler::stepsSamplePrep (Interface *interface, const DiskConfiguration &diskConfig, const SamplePrepConfiguration &samplePrepConfig, ProgressTracker *tracker)
{ if (samplePrepConfig.enable ())
  { tracker->mark (Progress (Progress::SamplePrep, Progress::InductionHeating, 0), 1);
    if (!interface->schPauseAddPause ()) return false;

    tracker->mark (Progress (Progress::SamplePrep, Progress::None, 0));
    if (!stepsBurnValve (interface, Progress::SamplePrep, diskConfig.hyperbaricValve (), tracker)) return false;  // Ming: 1st valve
  }

  return true;
}

bool Scheduler::stepsMetering (Interface *interface, const DiskConfiguration &diskConfig, const SamplePrepConfiguration &samplePrepConfig, const MeteringConfiguration &meteringConfig, ProgressTracker *tracker)
{ if (samplePrepConfig.enable ())
  { tracker->mark (Progress (Progress::Metering, Progress::None, 0), 2);

    if (!interface->schSpindleSetAccelDecel (meteringConfig.acceleration (), meteringConfig.deceleration ())) return false;
    if (!stepsSpinStart (interface, meteringConfig.evacuationVelocity (), meteringConfig.evacuationTime (), tracker)) return false;
    if (!stepsSpinStart (interface, meteringConfig.meteringVelocity (), meteringConfig.meteringTime (), tracker)) return false;
    if (!stepsSpinStop (interface, tracker)) return false;
    if (!interface->schSpindleSetAccelDecel (defaultAcceleration, defaultDeceleration)) return false;

    if (!stepsBurnValve (interface, Progress::Metering, diskConfig.meteringValve (), tracker)) return false;   // Ming: 2nd valve
  }

  return true;
}

bool Scheduler::stepsResuspension (Interface *interface, const DiskConfiguration &diskConfig, const SamplePrepConfiguration &samplePrepConfig, const ResuspensionConfiguration &resuspensionConfig, ProgressTracker *tracker)
{ if (samplePrepConfig.enable ())
  { tracker->mark (Progress (Progress::Resuspension, Progress::None, 0), 2);

    if (!interface->schSpindleSetAccelDecel (resuspensionConfig.acceleration (), resuspensionConfig.deceleration ())) return false;
    if (!stepsSpinStart (interface, resuspensionConfig.velocity (), resuspensionConfig.time (), tracker)) return false;
    if (!stepsSpinStop (interface, tracker)) return false;
    if (!interface->schSpindleSetAccelDecel (defaultAcceleration, defaultDeceleration)) return false;

    if (!stepsResuspensionMixing (interface, resuspensionConfig.mixing (), tracker)) return false;

    if (!stepsBurnValve (interface, Progress::Resuspension, diskConfig.insertValve (), tracker)) return false;
  }

  return true;
}

bool Scheduler::stepsFilling (Interface *interface, const DiskConfiguration &diskConfig, const FillingConfiguration &fillingConfig, const double standbyTemperatureSealer, ProgressTracker *tracker)
{ tracker->mark (Progress (Progress::Filling, Progress::None, 0), 2);

  if (!interface->schSpindleSetAccelDecel (fillingConfig.acceleration (), fillingConfig.deceleration ())) return false;
  if (!stepsSpinStart (interface, fillingConfig.velocity (), fillingConfig.time (), tracker)) return false;
  if (!stepsSpinStop (interface, tracker)) return false;
  if (!interface->schSpindleSetAccelDecel (defaultAcceleration, defaultDeceleration)) return false;

  if (!stepsSpindleHome (interface, Progress::Filling, tracker)) return false;

  tracker->mark (Progress (Progress::Filling, Progress::None, -1), 2);

  if (!interface->schHeaterOnSealer (fillingConfig.sealerTemperature (), Cycler::heatSealerRate)) return false;
  if (!interface->schHeaterWaitTemperatureSealer ()) return false;

  if (fillingConfig.reverse ())
  { if (!stepsSealingReverse (interface, diskConfig, fillingConfig, tracker)) return false;
  } else
  { if (!stepsSealingForward (interface, diskConfig, fillingConfig, tracker)) return false;
  }

  tracker->append (1);
  if (!qIsNaN (standbyTemperatureSealer))
  { if (!interface->schHeaterOnSealer (standbyTemperatureSealer, Cycler::heatSealerRate)) return false;
  } else
  { if (!interface->schHeaterOffSealer ()) return false;
  }

  return true;
}

bool Scheduler::stepsPcrPause (Interface *interface, ProgressTracker *tracker)
{ tracker->append (2);
  if (!interface->schStepperHome (false)) return false;
  if (!interface->schStepperWaitHome ()) return false;
  //if (!interface->schDoorOpen ()) return false;

  tracker->mark (Progress (Progress::PcrPausing, Progress::None, 0), 1);
  if (!interface->schPauseAddPause ()) return false;

  tracker->mark (Progress (Progress::Rt, Progress::None, 0), 2);
  if (!interface->schStepperHome (true)) return false;
  if (!interface->schStepperWaitHome ()) return false;

  if (!stepsSpindleHome (interface, Progress::Rt, tracker)) return false;

  return true;
}

bool Scheduler::stepsPreHbhPause (Interface *interface, ProgressTracker *tracker)
{ tracker->append (2);
  if (!interface->schStepperHome (false)) return false;
  if (!interface->schStepperWaitHome ()) return false;

  tracker->mark (Progress (Progress::PreHbhPausing, Progress::None, 0), 1);
  if (!interface->schPauseAddPause ()) return false;

  tracker->mark (Progress (Progress::PreTest, Progress::None, 0), 2);
  if (!interface->schStepperHome (true)) return false;
  if (!interface->schStepperWaitHome ()) return false;

  if (!stepsSpindleHome (interface, Progress::PreTest, tracker)) return false;

  return true;
}

bool Scheduler::stepsRt (Interface *interface, const DiskConfiguration &diskConfig, const RtConfiguration &rtConfig, const ThermocyclingConfiguration &thermocyclingConfig, ProgressTracker *tracker)
{ if (rtConfig.rtEnable ())
  { tracker->mark (Progress (Progress::Rt, Progress::None, 0), 11);

    if (!interface->schSpindleMoveAngleAbsolute (diskConfig.heater65LocationTicks (), true)) return false;
    if (!interface->schSpindleWaitPosition ()) return false;
    if (!interface->schHeaterOn65c (rtConfig.rtTemperature (), Cycler::pcrHeaterRate)) return false;
    if (!interface->schHeaterWaitTemperature65c ()) return false;
    if (!interface->schGripper65cToPos (Interface::Closed)) return false;
    if (!interface->schGripperWaitForce65c ()) return false;
    if (!interface->schTimerStart1 ()) return false;
    if (!interface->schTimerWait1 (qRound (rtConfig.rtTime () * 1000.0))) return false;
    if (!interface->schGripper65cToPos (Interface::MoreOpen)) return false;
    if (!interface->schGripperWaitPosition65c ()) return false;

    if (!interface->schHeaterOn65c (thermocyclingConfig.annealingTemperature (), Cycler::pcrHeaterRate)) return false;
  }

  tracker->mark (Progress (Progress::Rt, Progress::None, 1), 10);

  if (!interface->schSpindleMoveAngleAbsolute (diskConfig.heater95LocationTicks (), true)) return false;
  if (!interface->schSpindleWaitPosition ()) return false;
  if (!interface->schHeaterOn95c (rtConfig.activationTemperature (), Cycler::pcrHeaterRate)) return false;
  if (!interface->schHeaterWaitTemperature95c ()) return false;
  if (!interface->schGripper95cToPos (Interface::Closed)) return false;
  if (!interface->schGripperWaitForce95c ()) return false;
  if (!interface->schTimerStart1 ()) return false;
  if (!interface->schTimerWait1 (qRound (rtConfig.activationTime () * 1000.0))) return false;
  if (!interface->schGripper95cToPos (Interface::MoreOpen)) return false;
  if (!interface->schGripperWaitPosition95c ()) return false;

  return true;
}

bool Scheduler::stepsThermocycling (Interface *interface, const DiskConfiguration &diskConfig, const ThermocyclingConfiguration &config, ProgressTracker *tracker)
{ tracker->mark (Progress (Progress::Thermocycling, Progress::None, 0), 7);

  // Turn on heaters.
  if (!interface->schHeaterOn65c (config.annealingTemperature (), Cycler::pcrHeaterRate)) return false;
  if (!interface->schHeaterOn95c (config.denaturationTemperature (), Cycler::pcrHeaterRate)) return false;
  if (!interface->schHeaterWaitTemperature65c ()) return false;
  if (!interface->schHeaterWaitTemperature95c ()) return false;

  // Move spindle to heater 95.
  if (!interface->schSpindleMoveAngleAbsolute (diskConfig.heater95LocationTicks (), true)) return false;
  if (!interface->schSpindleWaitPosition ()) return false;

  // Start of loop.
  if (!interface->schLoopSetMarker ()) return false;

  /* Thermocycling time reduction. Ming: 12/12/2023
   * 1. Move the disc away to the 2nd heating block immediate after the 1st gripper leaves the heating block,
   *    without waiting for the completion of the opening of the 1st gripper.
   * 2. Close the 2nd gripper immediately after the disc moves away from the 1st heating block, without
   *    waiting for the disc final position reached
   */
  for (int cycle = 1; cycle <= config.numOfCycles (); cycle++)
  { tracker->mark (Progress (Progress::Thermocycling, Progress::Closing95, cycle), 2);
    tracker->mark (Progress (Progress::Thermocycling, Progress::Waiting95, cycle), 2);
    tracker->mark (Progress (Progress::Thermocycling, Progress::Opening95, cycle), 3);
    tracker->mark (Progress (Progress::Thermocycling, Progress::None, cycle), 1);

    tracker->mark (Progress (Progress::Thermocycling, Progress::Closing65, cycle), 2);
    tracker->mark (Progress (Progress::Thermocycling, Progress::Waiting65, cycle), 2);
    tracker->mark (Progress (Progress::Thermocycling, Progress::Opening65, cycle), 3);
    tracker->mark (Progress (Progress::Thermocycling, Progress::None, cycle), 2);
  }

  // Run denaturation cycle.
  if (!interface->schGripper95cToPos (Interface::Closed)) return false;
  if (!interface->schGripperWaitForce95c ()) return false;
  if (!interface->schTimerStart1 ()) return false;
  if (!interface->schTimerWait1 (qRound (config.denaturationTime () * 1000.0))) return false;
  if (!interface->schGripper95cToPos (Interface::SlightlyOpen)) return false;
  if (!interface->schTimerStart1 ()) return false;  // Wait for the gripper leaves the disc
  if (!interface->schTimerWait1 (80)) return false; // ms

  // Move spindle to heater 65.
  if (!interface->schSpindleMoveAngleAbsolute (diskConfig.heater65LocationTicks (), true)) return false;

  // Run annealing cycle.
  if (!interface->schGripper65cToPos (Interface::Closed)) return false;
  if (!interface->schGripperWaitForce65c ()) return false;
  if (!interface->schTimerStart1 ()) return false;
  if (!interface->schTimerWait1 (qRound (config.annealingTime () * 1000.0))) return false;
  if (!interface->schGripper65cToPos (Interface::SlightlyOpen)) return false;
  if (!interface->schTimerStart1 ()) return false;  // Wait for the gripper leaves the disc
  if (!interface->schTimerWait1 (80)) return false; // ms

  // Move spindle to heater 95 (with trigger for optics board).
  if (!interface->schSpindleMoveAngleAbsoluteOpto (diskConfig.heater95LocationTicks (), true)) return false;

  // End of loop.
  if (!interface->schLoopGotoMarker (config.numOfCycles ())) return false;

  tracker->append (4);

  // Open grippers.
  if (!interface->schGripperAllToPos (Interface::FullOpen)) return false;
  if (!interface->schGripperWaitPosition65c ()) return false;
  if (!interface->schGripperWaitPosition95c ()) return false;
  if (!interface->schGripperWaitPositionSealer ()) return false;

  return true;
}

bool Scheduler::stepsPostTest (Interface *interface, const double standbyTemperature65, const double standbyTemperature95, ProgressTracker *tracker)
{ tracker->mark (Progress (Progress::PostTest, Progress::None, 0), 6);

  // Stop spindle.
  if (!interface->schSpindleDisable ()) return false;

  // Retract gantry.
  if (!interface->schStepperHome (false)) return false;
//  if (!interface->schStepperMovePositionAbsolute (900000, 60000)) return false;

  // Run heaters on standby temperature.
  if (!qIsNaN (standbyTemperature65))
  { if (!interface->schHeaterOn65c (standbyTemperature65, Cycler::pcrHeaterRate)) return false;
  } else
  { if (!interface->schHeaterOff65c ()) return false;
  }

  if (!qIsNaN (standbyTemperature95))
  { if (!interface->schHeaterOn95c (standbyTemperature95, Cycler::pcrHeaterRate)) return false;
  } else
  { if (!interface->schHeaterOff95c ()) return false;
  }

  if (!interface->schStepperWaitHome ()) return false;

  // Open door.
  if (!interface->schDoorOpen ()) return false;

  return true;
}

bool Scheduler::stepsSealingForward (Interface *interface, const DiskConfiguration &diskConfig, const FillingConfiguration &config, ProgressTracker *tracker)
// Selectable sealer and neck sealer.
{ for (int cuvette = 0; cuvette <= 8; cuvette ++)
  { // Check sealer position mask
    if ((config.sealerEnable () & (1 << cuvette)) != 0)
    { // Sealer position is enabled
      tracker->mark (Progress (Progress::Filling, Progress::None, cuvette), 8);

      if (!interface->schSpindleMoveAngleAbsolute (diskConfig.cuvetteLocationTicks (cuvette), true)) return false;
      if (!interface->schSpindleWaitPosition ()) return false;
      if (!interface->schGripperSealerToPos (Interface::Closed)) return false;
      if (!interface->schGripperWaitForceSealer ()) return false;
      if (!interface->schTimerStart1 ()) return false;
      if (!interface->schTimerWait1 (qRound (config.sealingTime () * 1000.0))) return false;
      if (!interface->schGripperSealerToPos (cuvette < 9?Interface::MoreOpen:Interface::FullOpen)) return false;
      if (!interface->schGripperWaitPositionSealer ()) return false;
    }
  }

  return true;
}

bool Scheduler::stepsSealingReverse (Interface *interface, const DiskConfiguration &diskConfig, const FillingConfiguration &config, ProgressTracker *tracker)
{ for (int cuvette = 8; cuvette >= 0; cuvette -= 2)
  { tracker->mark (Progress (Progress::Filling, Progress::None, cuvette), 8);

    if (!interface->schSpindleMoveAngleAbsolute (diskConfig.cuvetteLocationTicks (cuvette), false)) return false;
    if (!interface->schSpindleWaitPosition ()) return false;
    if (!interface->schGripperSealerToPos (Interface::Closed)) return false;
    if (!interface->schGripperWaitForceSealer ()) return false;
    if (!interface->schTimerStart1 ()) return false;
    if (!interface->schTimerWait1 (qRound (config.sealingTime () * 1000.0))) return false;
    if (!interface->schGripperSealerToPos (cuvette > 0?Interface::MoreOpen:Interface::FullOpen)) return false;
    if (!interface->schGripperWaitPositionSealer ()) return false;
  }

  return true;
}

bool Scheduler::stepsSpindleHome (Interface *interface, const Progress::State state, ProgressTracker *tracker)
{ tracker->mark (Progress (state, Progress::Homing, 0), 8);

  if (!interface->schSpindleHome (Interface::Disk)) return false;
  if (!interface->schSpindleWaitPosition ()) return false;
  if (!interface->schTimerStart1 ()) return false;
  if (!interface->schTimerWait1 (500)) return false;

  if (!interface->schSpindleHome (Interface::Disk)) return false;
  if (!interface->schSpindleWaitPosition ()) return false;
  if (!interface->schTimerStart1 ()) return false;
  if (!interface->schTimerWait1 (500)) return false;

  return true;
}

bool Scheduler::stepsSpinStart (Interface *interface, const int velocity, const double time, ProgressTracker *tracker)
{ tracker->append (4);

  if (!interface->schSpindleMoveSpeed (velocity)) return false;
  if (!interface->schSpindleWaitSpeed ()) return false;
  if (!interface->schTimerStart1 ()) return false;
  if (!interface->schTimerWait1 (qRound (time * 1000.0))) return false;

  return true;
}

bool Scheduler::stepsSpinStop (Interface *interface, ProgressTracker *tracker)
{ tracker->append (4);

  if (!interface->schSpindleStop ()) return false;
  if (!interface->schSpindleWaitSpeed ()) return false;
  if (!interface->schTimerStart1 ()) return false;
  if (!interface->schTimerWait1 (500)) return false;

  return true;
}

bool Scheduler::stepsPosition (Interface *interface, const int position, const bool direction, ProgressTracker *tracker)
{ tracker->append (2);

  if (!interface->schSpindleMoveAngleAbsolute_Ming (position, direction)) return false;
  if (!interface->schSpindleWaitPosition ()) return false;

  return true;
}

bool Scheduler::stepsBurnValve (Interface *interface, const Progress::State state, const Valve &valve, ProgressTracker *tracker)
{ if (!stepsSpindleHome (interface, state, tracker)) return false;
  if (!stepsPosition (interface, valve.angleTicks (), true, tracker)) return false;

  tracker->mark (Progress (state, Progress::None, 0), 2);
  if (!interface->schStepperMovePositionRelative (-valve.radiusSteps (), 30000)) return false;
  if (!interface->schStepperWaitPosition ()) return false;

  tracker->mark (Progress (state, Progress::FiringLaser, 0), 1);
  if (!interface->schPauseAddPause ()) return false;

  tracker->mark (Progress (state, Progress::None, 0), 2);
  if (!interface->schStepperMovePositionRelative (valve.radiusSteps (), 30000)) return false;
  if (!interface->schStepperWaitPosition ()) return false;

  return true;
}

bool Scheduler::stepsResuspensionMixing (Interface *interface, const MixingConfiguration &mixingConfig, ProgressTracker *tracker)
{ if (mixingConfig.numOfCycles () > 0)
  { tracker->append (3 + mixingConfig.numOfCycles () * 7);

    if (!interface->schSpindleSetAccelDecel (mixingConfig.acceleration (), mixingConfig.deceleration ())) return false;
    if (!interface->schLoopSetMarker ()) return false;

    if (!interface->schSpindleMoveSpeed (mixingConfig.velocity ())) return false;
    if (!interface->schTimerStart0 ()) return false;
    if (!interface->schTimerWait0 (qRound (mixingConfig.interval () * 1000.0))) return false;

    if (!interface->schSpindleMoveSpeed (-mixingConfig.velocity ())) return false;
    if (!interface->schTimerStart0 ()) return false;
    if (!interface->schTimerWait0 (qRound (mixingConfig.interval () * 1000.0))) return false;

    if (!interface->schLoopGotoMarker (mixingConfig.numOfCycles ())) return false;
    if (!interface->schSpindleSetAccelDecel (defaultAcceleration, defaultDeceleration)) return false;
  }

  return true;
}

bool Scheduler::stepsPreHbhMixing (Interface *interface, const PreHbhMixingConfiguration &preHbhMixingConfig, ProgressTracker *tracker)
{ if (preHbhMixingConfig.numOfCycles () > 0)
  { tracker->append (3 + preHbhMixingConfig.numOfCycles () * 7);

    if (!interface->schSpindleSetAccelDecel (preHbhMixingConfig.acceleration (), preHbhMixingConfig.deceleration ())) return false;
    if (!interface->schLoopSetMarker ()) return false;

    if (!interface->schSpindleMoveSpeed (preHbhMixingConfig.velocity ())) return false;
    if (!interface->schTimerStart0 ()) return false;
    if (!interface->schTimerWait0 (qRound (preHbhMixingConfig.interval () * 1000.0))) return false;

    if (!interface->schSpindleMoveSpeed (-preHbhMixingConfig.velocity ())) return false;
    if (!interface->schTimerStart0 ()) return false;
    if (!interface->schTimerWait0 (qRound (preHbhMixingConfig.interval () * 1000.0))) return false;

    if (!interface->schLoopGotoMarker (preHbhMixingConfig.numOfCycles ())) return false;
    if (!interface->schSpindleSetAccelDecel (defaultAcceleration, defaultDeceleration)) return false;
  }

  return true;
}
