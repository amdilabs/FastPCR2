#include "progress.h"

Progress::Progress (void) : m_state (Unknown), m_subState (None), m_index (-1)
{
}

Progress::Progress (const State state, const SubState subState, const int index) : m_state (state), m_subState (subState), m_index (index)
{
}

Progress::State Progress::state (void) const
{ return m_state;
}

Progress::SubState Progress::subState (void) const
{ return m_subState;
}

int Progress::index (void) const
{ return m_index;
}
