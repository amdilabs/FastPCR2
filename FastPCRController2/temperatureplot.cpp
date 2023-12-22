#include "temperatureplot.h"

const int TemperaturePlot::size = 1000;

const QColor TemperaturePlot::colours [] = {
  QColor (0x00, 0x3f, 0x5c),
  QColor (0x2f, 0x4b, 0x7c),
  QColor (0x66, 0x51, 0x91),
  QColor (0xa0, 0x51, 0x95),
  QColor (0xd4, 0x50, 0x87),
  QColor (0xf9, 0x5d, 0x6a),
  QColor (0xff, 0x7c, 0x43),
  QColor (0xff, 0xa6, 0x00)
};

const int TemperaturePlot::numOfColours = sizeof (colours) / sizeof (QColor);

TemperaturePlot::TemperaturePlot (QWidget *parent) : CustomQCustomPlot (parent)
{ m_index = 0;

  QPen pen;
  //pen.setWidth (2);

  xAxis->setLabel ("Sample");
  yAxis->setLabel ("Temperature (°C)");

  pen.setColor (getColour ());
  m_graphTop65 [0] = addGraph (xAxis, yAxis);
  m_graphTop65 [0]->setName ("top65 (rtd1)");
  m_graphTop65 [0]->setPen (pen);

  pen.setColor (getColour ());
  m_graphTop65 [1] = addGraph (xAxis, yAxis);
  m_graphTop65 [1]->setName ("top65 (rtd2)");
  m_graphTop65 [1]->setPen (pen);

  pen.setColor (getColour ());
  m_graphTop65 [2] = addGraph (xAxis, yAxis);
  m_graphTop65 [2]->setName ("top65 (th)");
  m_graphTop65 [2]->setPen (pen);

  pen.setColor (getColour ());
  m_graphBottom65 [0] = addGraph (xAxis, yAxis);
  m_graphBottom65 [0]->setName ("bottom65 (rtd1)");
  m_graphBottom65 [0]->setPen (pen);

  pen.setColor (getColour ());
  m_graphBottom65 [1] = addGraph (xAxis, yAxis);
  m_graphBottom65 [1]->setName ("bottom65 (rtd2)");
  m_graphBottom65 [1]->setPen (pen);

  pen.setColor (getColour ());
  m_graphBottom65 [2] = addGraph (xAxis, yAxis);
  m_graphBottom65 [2]->setName ("bottom65 (th)");
  m_graphBottom65 [2]->setPen (pen);

  pen.setColor (getColour ());
  m_graphTop95 [0] = addGraph (xAxis, yAxis);
  m_graphTop95 [0]->setName ("top95 (rtd1)");
  m_graphTop95 [0]->setPen (pen);

  pen.setColor (getColour ());
  m_graphTop95 [1] = addGraph (xAxis, yAxis);
  m_graphTop95 [1]->setName ("top95 (rtd2)");
  m_graphTop95 [1]->setPen (pen);

  pen.setColor (getColour ());
  m_graphTop95 [2] = addGraph (xAxis, yAxis);
  m_graphTop95 [2]->setName ("top95 (th)");
  m_graphTop95 [2]->setPen (pen);

  pen.setColor (getColour ());
  m_graphBottom95 [0] = addGraph (xAxis, yAxis);
  m_graphBottom95 [0]->setName ("bottom95 (rtd1)");
  m_graphBottom95 [0]->setPen (pen);

  pen.setColor (getColour ());
  m_graphBottom95 [1] = addGraph (xAxis, yAxis);
  m_graphBottom95 [1]->setName ("bottom95 (rtd2)");
  m_graphBottom95 [1]->setPen (pen);

  pen.setColor (getColour ());
  m_graphBottom95 [2] = addGraph (xAxis, yAxis);
  m_graphBottom95 [2]->setName ("bottom95 (th)");
  m_graphBottom95 [2]->setPen (pen);

  pen.setColor (getColour ());
  m_graphHeatSealer = addGraph (xAxis, yAxis);
  m_graphHeatSealer->setName ("heat sealer");
  m_graphHeatSealer->setPen (pen);

  legend->setVisible (true);

  m_index = 0;
  m_used = 0;

  for (int index = 0; index < 3; index++)
  { m_top65 [index] = new double [size];
    m_bottom65 [index] = new double [size];
    m_top95 [index] = new double [size];
    m_bottom95 [index] = new double [size];
  }

  m_heatSealer = new double [size];

  zoomAll ();
}

TemperaturePlot::~TemperaturePlot (void)
{ for (int index = 0; index < 3; index++)
  { delete [] m_top65 [index];
    delete [] m_bottom65 [index];
    delete [] m_top95 [index];
    delete [] m_bottom95 [index];
  }

  delete [] m_heatSealer;
}

void TemperaturePlot::zoomAll (void)
{ CustomQCustomPlot::zoomAll ();

  xAxis->setRange (-(m_used - 1), 0.0);
  yAxis->setRange (0.0, 250.0);

  replot ();
}

void TemperaturePlot::temperaturesReceived (const QList<double> &top65, const QList<double> &bottom65, const QList<double> &top95, const QList<double> &bottom95, const double heatSealer)
{ for (int index = 0; index < 3; index++)
  { m_top65 [index][m_index] = top65.at (index);
    m_bottom65 [index][m_index] = bottom65.at (index);
    m_top95 [index][m_index] = top95.at (index);
    m_bottom95 [index][m_index] = bottom95.at (index);
  }

  m_heatSealer [m_index] = heatSealer;

  if (++m_index == size) m_index = 0;
  if (m_used < size)
  { m_x.prepend (-m_used);
    m_used++;
  }

  for (int index = 0; index < 3; index++)
  { m_graphTop65 [index]->setData (m_x, vec (m_top65 [index]), true);
    m_graphBottom65 [index]->setData (m_x, vec (m_bottom65 [index]), true);
    m_graphTop95 [index]->setData (m_x, vec (m_top95 [index]), true);
    m_graphBottom95 [index]->setData (m_x, vec (m_bottom95 [index]), true);
  }

  m_graphHeatSealer->setData (m_x, vec (m_heatSealer), true);

  if (zoomedOut ()) zoomAll (); else replot ();
}

QColor TemperaturePlot::getColour (void)
{ if (m_index == numOfColours) m_index = 0;
  return colours [m_index++];
}

QVector<double> TemperaturePlot::vec (double *data) const
{ QVector<double> result;
  result.reserve (m_used);

  int ptr = m_index - m_used;
  if (ptr < 0) ptr += size;

  for (int index = 0; index < m_used; index++)
  { result.append (data [ptr]);
    if (++ptr == size) ptr = 0;
  }

  return result;
}
