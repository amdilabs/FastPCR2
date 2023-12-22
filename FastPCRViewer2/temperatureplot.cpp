#include "../ui/colourmap.h"
#include "temperatureplot.h"

TemperaturePlot::TemperaturePlot(QWidget *parent) : CustomQCustomPlot (parent), m_timeMin (0.0), m_timeMax (100.0)
{ xAxis->setLabel ("Time (s)");
  yAxis->setLabel ("Temperature (°C)");
  legend->setVisible (true);
  TemperaturePlot::zoomAll ();
}

void TemperaturePlot::zoomAll (void)
{ CustomQCustomPlot::zoomAll ();
  xAxis->setRange (m_timeMin, m_timeMax);
  yAxis->setRange (0.0, 250.0);
  replot ();
}

void TemperaturePlot::plot (const TemperatureData &data)
{ clearGraphs ();

  QList<double> time = data.time ();
  if (!time.isEmpty ())
  { m_timeMin = time.first ();
    m_timeMax = time.last ();
  }

  ColourMap map;
  addGraph (map.colour (), "Top 65 (RTD1)", data.time (), data.top65Rtd1 ()); map.next ();
  addGraph (map.colour (), "Top 65 (RTD2)", data.time (), data.top65Rtd2 ()); map.next ();
  addGraph (map.colour (), "Top 65 (Thermistor)", data.time (), data.top65Thermistor ()); map.next ();
  addGraph (map.colour (), "Bottom 65 (RTD1)", data.time (), data.bottom65Rtd1 ()); map.next ();
  addGraph (map.colour (), "Bottom 65 (RTD2)", data.time (), data.bottom65Rtd2 ()); map.next ();
  addGraph (map.colour (), "Bottom 65 (Thermistor)", data.time (), data.bottom65Thermistor ()); map.next ();
  addGraph (map.colour (), "Top 95 (RTD1)", data.time (), data.top95Rtd1 ()); map.next ();
  addGraph (map.colour (), "Top 95 (RTD2)", data.time (), data.top95Rtd2 ()); map.next ();
  addGraph (map.colour (), "Top 95 (Thermistor)", data.time (), data.top95Thermistor ()); map.next ();
  addGraph (map.colour (), "Bottom 95 (RTD1)", data.time (), data.bottom95Rtd1 ()); map.next ();
  addGraph (map.colour (), "Bottom 95 (RTD2)", data.time (), data.bottom95Rtd2 ()); map.next ();
  addGraph (map.colour (), "Bottom 95 (Thermistor)", data.time (), data.bottom95Thermistor ()); map.next ();
  addGraph (map.colour (), "Heat sealer", data.time (), data.heatSealer ());

  if (zoomedOut ()) zoomAll (); else replot ();
}

void TemperaturePlot::addGraph (const QColor &colour, const QString &name, const QList<double> &x, const QList<double> &y)
{ QPen pen (colour);
  pen.setWidth (2);

  QCPGraph *g = CustomQCustomPlot::addGraph (xAxis, yAxis);
  g->setPen (pen);
  g->setName (name);
  g->setData (x, y, true);
}
