#include "colourmap.h"
#include "pcrprofileplot.h"

PcrProfilePlot::PcrProfilePlot (QWidget *parent) : CustomQCustomPlot (parent), m_channelMask (0xffffffff), m_cuvetteMask (0xffffffff), m_maxNumOfCycles (50)
{ xAxis->setLabel ("Cycle");
  yAxis->setLabel ("Voltage (mV)");
  legend->setVisible (true);

  PcrProfilePlot::zoomAll ();
}

void PcrProfilePlot::zoomAll (void)
{ CustomQCustomPlot::zoomAll ();

  xAxis->setRange (0.0, m_maxNumOfCycles);
  yAxis->setRange (-100.0, 3400.0);

  replot ();
}

int PcrProfilePlot::channelMask (void) const
{ return m_channelMask;
}

int PcrProfilePlot::cuvetteMask (void) const
{ return m_cuvetteMask;
}

void PcrProfilePlot::setChannelMask (const int channelMask)
{ if (m_channelMask != channelMask)
  { m_channelMask = channelMask;
    plot (m_profiles);
  }
}

void PcrProfilePlot::setCuvetteMask (const int cuvetteMask)
{ if (m_cuvetteMask != cuvetteMask)
  { m_cuvetteMask = cuvetteMask;
    plot (m_profiles);
  }
}

void PcrProfilePlot::plot (const QList<PcrProfile> &profiles)
{ m_profiles = profiles;

  clearGraphs ();

  ColourMap map;
  m_maxNumOfCycles = 50;

  for (const PcrProfile &profile : profiles)
  { if (profile.id ().match (m_channelMask, m_cuvetteMask))
    { addGraph (profile, map.colour (), map.shape ());
      map.next ();

      m_maxNumOfCycles = qMax (m_maxNumOfCycles, profile.numOfCycles ());
    }
  }

  if (zoomedOut ()) zoomAll (); else replot ();
}

void PcrProfilePlot::addGraph (const PcrProfile &profile, const QColor &colour, const QCPScatterStyle::ScatterShape shape)
{ QPen pen (colour);
  pen.setWidth (2);

  QCPGraph *g = CustomQCustomPlot::addGraph (xAxis, yAxis);
  g->setPen (pen);
  g->setScatterStyle (QCPScatterStyle (shape, 5.0));
  g->setName (profile.id ().name ());
  g->setData (x (profile.numOfCycles ()), y (profile.values ()), true);
}

QList<double> PcrProfilePlot::x (const int numOfCycles)
{ QList<double> result; result.reserve (numOfCycles);
  for (int cycle = 1; cycle <= numOfCycles; cycle++) result.append (cycle);
  return result;
}

QList<double> PcrProfilePlot::y (const QList<double> &values)
{ QList<double> result; result.reserve (values.count ());
  for (const double value : values) result.append (value * 1000.0);
  return result;
}
