#include "processor.h"
#include "plot.h"

Plot::Plot (QWidget *parent) : CustomQCustomPlot (parent)
{ xAxis->setLabel ("Time (s)");
  yAxis->setLabel ("Temperature (°C)");
  legend->setVisible (true);

  clear ();
  Plot::zoomAll ();
}

void Plot::zoomAll (void)
{ CustomQCustomPlot::zoomAll ();

  xAxis->setRange (m_timeMin, m_timeMax);

  double margin = (m_temperatureMax - m_temperatureMin) * 0.1;
  yAxis->setRange (m_temperatureMin - margin, m_temperatureMax + margin);

  replot ();
}

void Plot::clear (void)
{ m_timeMin = 0.0;
  m_timeMax = 10.0;
  m_temperatureMin = 20.0;
  m_temperatureMax = 50.0;

  m_graphs.clear ();
  m_lines.clear ();

  clearGraphs ();
  clearItems ();
  replot ();
  m_map.reset ();
}

void Plot::clearLast (void)
{ if (!m_graphs.isEmpty ())
  { removeGraph (m_graphs.takeLast ());
    removeItem (m_lines.takeLast ());

    replot ();
  }
}

void Plot::plot (const Data &data, const double offset, const Ramp &ramp)
{ QPen pen;
  pen.setColor (m_map.colour ());
  pen.setWidth (2);
  m_map.next ();

  QList<double> time = data.time (offset);
  m_timeMin = qMin (m_timeMin, time.first ());
  m_timeMax = qMax (m_timeMax, time.last ());

  QList<double> temperature = data.temperature ();
  for (const double value : temperature)
  { m_temperatureMin = qMin (m_temperatureMin, value);
    m_temperatureMax = qMax (m_temperatureMax, value);
  }

  QCPGraph *graph = addGraph (xAxis, yAxis);
  graph->setPen (pen);
  graph->setName (data.name () + " (" + QString::number (ramp.rate (), 'f', 2) + " °C/s)");
  graph->setData (time, data.temperature ());
  graph->setSelectable (QCP::stWhole);
  m_graphs.append (graph);

  pen.setWidth (1);
  pen.setStyle (Qt::DashLine);

  QCPItemStraightLine *line = new QCPItemStraightLine (this);
  line->setPen (pen);
  line->point1->setCoords (-offset, ramp.origin ());
  line->point2->setCoords (-offset + 1.0, ramp.origin () + ramp.rate ());
  m_lines.append (line);

  if (zoomedOut ()) zoomAll (); else replot ();
}
