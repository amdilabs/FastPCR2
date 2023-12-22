#include "../ui/colourmap.h"
#include "spatialdataplot.h"

SpatialDataPlot::SpatialDataPlot (QWidget *parent) : CustomQCustomPlot (parent), m_channelMask (0xffffffff), m_positionMin (0), m_positionMax (360.0)
{ xAxis->setLabel ("Angular position (°)");
  yAxis->setLabel ("Voltage (mV)");

  legend->setVisible (true);

  SpatialDataPlot::zoomAll ();
}

void SpatialDataPlot::zoomAll (void)
{ CustomQCustomPlot::zoomAll ();

  xAxis->setRange (m_positionMin, m_positionMax);
  yAxis->setRange (-100.0, 3400.0);

  replot ();
}

int SpatialDataPlot::channelMask (void) const
{ return m_channelMask;
}

void SpatialDataPlot::setChannelMask (const int channelMask)
{ if (m_channelMask != channelMask)
  { m_channelMask = channelMask;
    plot (m_data);
  }
}

CuvetteLocationConfigurationContainer SpatialDataPlot::cuvetteConfig (void) const
{ return m_cuvetteConfig;
}

void SpatialDataPlot::setCuvetteConfig (const CuvetteLocationConfigurationContainer &config)
{ m_cuvetteConfig = config;
  plot (m_data);
}

void SpatialDataPlot::plot (const QList<SpatialData> &data)
{ m_data = data;

  clearGraphs ();
  clearItems ();

  QCPGraph *g;

  ColourMap map;
  QPen pen;

  m_positionMin = 1.0e100;
  m_positionMax = -1.0e100;

  for (const SpatialData &d : data)
  { if (m_channelMask & (1 << d.channel ()))
    { CuvetteLocationConfiguration config = m_cuvetteConfig.configuration (d.channel ());

      pen.setColor (map.colour ());
      pen.setStyle (Qt::NoPen);

      QColor colour = map.colour ();
      colour.setAlpha (16);

      QCPItemRect *r = new QCPItemRect (this);
      r->setPen (pen);
      r->setBrush (QBrush (colour));
      r->topLeft->setCoords (config.darkStart (), 3400.0);
      r->bottomRight->setCoords (config.darkEnd (), -100.0);

      QCPItemText *t = new QCPItemText (this);
      t->setColor (map.colour ());
      t->setText ("dark");
      t->setTextAlignment (Qt::AlignVCenter | Qt::AlignHCenter);
      t->position->setCoords (0.5 * (config.darkStart () + config.darkEnd ()), 3300.0 - (d.channel () * 100.0));

      for (int cuvette = 0; cuvette < config.numOfCuvettes (); cuvette++)
      { pen.setStyle (Qt::NoPen);

        QCPItemRect *r = new QCPItemRect (this);
        r->setPen (pen);
        r->setBrush (QBrush (colour));
        r->topLeft->setCoords (config.cuvetteStart (cuvette), 3400.0);
        r->bottomRight->setCoords (config.cuvetteEnd (cuvette), -100.0);

        pen.setStyle (Qt::DotLine);
        pen.setWidth (0);

        double loc = config.cuvetteLocation (cuvette);
        QCPItemStraightLine *l = new QCPItemStraightLine (this);
        l->setPen (pen);
        l->point1->setCoords (loc, 0.0);
        l->point2->setCoords (loc, 1.0);

        QCPItemText *t = new QCPItemText (this);
        t->setColor (map.colour ());
        t->setText (QString::number (cuvette + 1));
        t->setTextAlignment (Qt::AlignVCenter | Qt::AlignHCenter);
        t->position->setCoords (loc, 3300.0 - (d.channel () * 100.0));
      }

      if (d.numOfSamples () > 0)
      { pen.setStyle (Qt::SolidLine);
        pen.setWidth (2);

        QList<double> position = d.position ();

        g = addGraph (xAxis, yAxis);
        g->setPen (pen);
        g->setName ("PD" + QString::number (d.channel () + 1));
        g->setData (position, convertAdc (d.adc ()), true);

        m_positionMin = position.first ();
        m_positionMax = position.last ();
      }
    }

    map.next ();
  }

  if (zoomedOut ()) zoomAll (); else replot ();
}

QList<double> SpatialDataPlot::convertAdc (const QList<double> &list)
{ QList<double> result;
  result.reserve (list.count ());

  for (const double value : list) result.append (value * 1000.0);

  return result;
}
