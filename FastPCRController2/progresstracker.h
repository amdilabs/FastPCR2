#ifndef PROGRESSTRACKER_H
#define PROGRESSTRACKER_H

#include <QList>
#include "progress.h"

class ProgressTracker
{ public:
    ProgressTracker (void);

    void append (int numOfSteps);
    void clear (void);
    void mark (const Progress &progress, const int count = 0);

    Progress progress (const int step);
    int total (void) const;

  private:
    QList<Progress> m_steps;
    Progress m_mark;
};

#endif // PROGRESSTRACKER_H
