#include "../ui/colourmap.h"
#include "timedataplot.h"

TimeDataPlot::TimeDataPlot (QWidget *parent) : CustomQCustomPlot (parent), m_channelMask (0xffffffff), m_time (100.0), m_encoderMin (0), m_encoderMax (0)
{ xAxis->setLabel ("Time (ms)");
  yAxis->setLabel ("Voltage (mV)");
  yAxis2->setLabel ("Angular position (ticks)");
  yAxis2->setVisible (true);

  legend->setVisible (true);

  TimeDataPlot::zoomAll ();
}

void TimeDataPlot::zoomAll (void)
{ CustomQCustomPlot::zoomAll ();

  xAxis->setRange (0.0, m_time);
  yAxis->setRange (-100.0, 3400.0);
  yAxis2->setRange (m_encoderMin - 1000.0, m_encoderMax + 1000.0);

  replot ();
}

int TimeDataPlot::channelMask (void) const
{ return m_channelMask;
}

void TimeDataPlot::setChannelMask (const int channelMask)
{ if (m_channelMask != channelMask)
  { m_channelMask = channelMask;
    plot (m_data);
  }
}

void TimeDataPlot::plot (const QList<TimeData> &data)
{ m_data = data;

  clearGraphs ();

  QCPGraph *g;

  ColourMap map;
  QPen pen;
  pen.setWidth (2);

  for (const TimeData &d : data)
  { if ((m_channelMask & (1 << d.channel ())) && (d.numOfSamples () > 0))
    { QList<double> t = time (d.numOfSamples ());
      m_time = t.last ();

      pen.setColor (map.colour ());

      pen.setStyle (Qt::DotLine);
      g = addGraph (xAxis, yAxis2);
      g->setPen (pen);
      g->setName ("Encoder" + QString::number (d.channel () + 1));
      g->setData (t, convertEncoder (d.encoder (), &m_encoderMin, &m_encoderMax), true);

      pen.setStyle (Qt::SolidLine);
      g = addGraph (xAxis, yAxis);
      g->setPen (pen);
      g->setName ("PD" + QString::number (d.channel () + 1));
      g->setData (t, convertAdc (d.adc ()), true);
    }

    map.next ();
  }

  if (zoomedOut ()) zoomAll (); else replot ();
}

QList<double> TimeDataPlot::time (const int numOfSamples)
{ QList<double> result;
  result.reserve (numOfSamples);

  for (int index = 0; index < numOfSamples; index++) result.append (static_cast<double> (index) * 0.01);

  return result;
}

QList<double> TimeDataPlot::convertEncoder (const QList<int> &list, int *min, int *max)
{ QList<double> result;
  result.reserve (list.count ());

  *min = 0x7fffffff;
  *max = -0x80000000;

  for (const int value : list)
  { result.append (value);

    *min = qMin (*min, value);
    *max = qMax (*max, value);
  }

  return result;
}

QList<double> TimeDataPlot::convertAdc (const QList<double> &list)
{ QList<double> result;
  result.reserve (list.count ());

  for (const double value : list) result.append (value * 1000.0);

  return result;
}
