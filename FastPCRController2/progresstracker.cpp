#include "progresstracker.h"

ProgressTracker::ProgressTracker (void)
{ clear ();
}

void ProgressTracker::append (int numOfSteps)
{ while (numOfSteps--) m_steps.append (m_mark);
}

void ProgressTracker::clear (void)
{ m_steps.clear ();
}

void ProgressTracker::mark (const Progress &progress, const int count)
{ m_mark = progress;
  append (count);
}

Progress ProgressTracker::progress (const int step)
{ return (step < m_steps.count ()?m_steps.at (step):m_steps.last ());
}

int ProgressTracker::total (void) const
{ return m_steps.count ();
}
