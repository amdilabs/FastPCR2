#ifndef PROGRESS_H
#define PROGRESS_H

class Progress
{ public:
    enum State {PreHbhSpinAndMixing, PreHbhPausing, PreTest, SamplePrep, Metering, Resuspension, Filling, PcrPausing, Rt, Thermocycling, PostTest, Unknown};
    enum SubState {None, Homing, InductionHeating, FiringLaser, Closing95, Waiting95, Opening95, Closing65, Waiting65, Opening65, ClosingHs, WaitingHs, OpeningHs};

    Progress (void);
    Progress (const State state, const SubState subState, const int index);

    State state (void) const;
    SubState subState (void) const;
    int index (void) const;

  private:
    State m_state;
    SubState m_subState;
    int m_index;
};

#endif // PROGRESS_H
