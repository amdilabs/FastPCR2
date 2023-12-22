#include "plotconfiguration.h"

PlotConfiguration::PlotConfiguration (void) : m_visible (0xffffffff)
{
}

bool PlotConfiguration::visible (const int index) const
{ return (m_visible & (1 << index)) != 0;
}

void PlotConfiguration::setVisible (const int index, const bool visible)
{ if (visible) m_visible |= (1 << index); else m_visible &= ~(1 << index);
}
