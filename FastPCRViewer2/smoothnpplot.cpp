#include "../ui/colourmap.h"
#include "smoothnpplot.h"

const int SmoothNpPlot::minNumOfCycles = 50;

SmoothNpPlot::SmoothNpPlot (QWidget *parent) : CustomQCustomPlot (parent), m_channelMask (0xffffffff), m_cuvetteMask (0xffffffff), m_numOfCycles (0)
{ QSharedPointer<QCPAxisTickerLog> ticker (new QCPAxisTickerLog);

  xAxis->setLabel ("Cycle");
  yAxis->setScaleType (QCPAxis::stLogarithmic);
  yAxis->setTicker (ticker);

  QPen pen (Qt::black);
  pen.setStyle (Qt::DotLine);
  pen.setWidth (2);

  m_line = new QCPItemStraightLine (this);
  m_line->setPen (pen);
  setThreshold (20.0);

  axisRect()->insetLayout()->setInsetAlignment (0, Qt::AlignLeft | Qt::AlignTop);

  legend->setVisible (true);
  SmoothNpPlot::zoomAll ();
}

void SmoothNpPlot::zoomAll (void)
{ CustomQCustomPlot::zoomAll ();

  xAxis->setRange (1.0, (m_numOfCycles > minNumOfCycles?m_numOfCycles:minNumOfCycles));
  yAxis->setRange (1.0, 10000.0);

  replot ();
}

int SmoothNpPlot::channelMask (void) const
{ return m_channelMask;
}

int SmoothNpPlot::cuvetteMask (void) const
{ return m_cuvetteMask;
}

void SmoothNpPlot::setChannelMask (const int channelMask)
{ if (m_channelMask != channelMask)
  { m_channelMask = channelMask;
    plot (m_cts);
  }
}

void SmoothNpPlot::setCuvetteMask (const int cuvetteMask)
{ if (m_cuvetteMask != cuvetteMask)
  { m_cuvetteMask = cuvetteMask;
    plot (m_cts);
  }
}

void SmoothNpPlot::plot (const QList<Ct> &cts)
{ m_cts = cts;

  clearGraphs ();

  m_numOfCycles = 0;

  ColourMap map;

  for (const Ct &ct : cts)
  { if (ct.id ().match (m_channelMask, m_cuvetteMask))
    { QPen pen (map.colour ());
      pen.setWidth (2);

      QVector<double> x;
      QVector<double> y;
      QList<double> smoothNp = ct.smoothNp ();
      for (int index = 0; index < smoothNp.count (); index++)
      { x.append (index + 1);
        y.append (smoothNp.at (index));
      }

      if (smoothNp.count () > m_numOfCycles) m_numOfCycles = smoothNp.count ();

      QCPGraph *g = addGraph (xAxis, yAxis);
      g->setPen (pen);
      g->setScatterStyle (QCPScatterStyle (map.shape (), 5.0));
      g->setName (ct.id ().name ());
      g->setData (x, y, true);
    }

    map.next ();
  }

  if (zoomedOut ()) zoomAll (); else replot ();
}

void SmoothNpPlot::setThreshold (const double threshold)
{ m_line->point1->setCoords (0.0, threshold);
  m_line->point2->setCoords (1000.0, threshold);
  replot ();
}
