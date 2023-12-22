#include "../../lib/math/regression.h"
#include "processor.h"

QList<QPointF> Processor::points1090 (const Data &data)
{ QList<double> temperature = data.temperature ();
  QList<double> time = data.time ();
  int index = peakIndex (temperature);

  if (index != -1)
  { double maxTemperature = temperature.at (index);
    double minTemperature = 1.0e100;

    int minIndex = index;
    for (; index>= 0; index--)
    { if (temperature.at (index) < minTemperature)
      { minTemperature = temperature.at (index);
        minIndex = index;
      }
    }

    double diff = maxTemperature - minTemperature;
    double temp10 = minTemperature + 0.1 * diff;
    double temp90 = minTemperature + 0.9 * diff;

    int index = minIndex;

    for (; (index < time.count ()) && (temperature.at (index) < temp10); index++);
    double time10 = interp (temperature.at (index - 1), temperature.at (index), time.at (index - 1), time.at (index), temp10);

    for (; (index < time.count ()) && (temperature.at (index) < temp90); index++);
    double time90 = interp (temperature.at (index - 1), temperature.at (index), time.at (index - 1), time.at (index), temp90);

    return QList<QPointF> () << QPointF (time10, temp10) << QPointF (time90, temp90);
  }

  return QList<QPointF> ();
}

QList<QPointF> Processor::pointsFromStart (const Data &data, const double step, const double length)
{ QList<double> time = data.time ();
  QList<double> temperature = data.temperature ();

  int start = startIndex (temperature);

  if (start != -1)
  { double t1 = time.at (start) + (step - length);
    double t2 = t1 + length;

    return QList<QPointF> () << find (t1, data) << find (t2, data);
  }

  return QList<QPointF> ();
}

QList<QPointF> Processor::pointsFromPeak (const Data &data, const double step, const double length)
{ QList<double> time = data.time ();
  QList<double> temperature = data.temperature ();

  int peak = peakIndex (temperature);

  if (peak != -1)
  { double t1 = time.at (peak) - step;
    double t2 = t1 + length;

    return QList<QPointF> () << find (t1, data) << find (t2, data);
  }

  return QList<QPointF> ();
}

Ramp Processor::twoPointRamp (const QList<QPointF> &points)
{ if (points.count () == 2)
  { double rate = (points.at (1).y () - points.at (0).y ()) / (points.at (1).x () - points.at (0).x ());
    double origin = points.at (0).y () - points.at (0).x () * rate;
    return Ramp (origin, rate);
  }

  return Ramp ();
}

Ramp Processor::linearRegressionRamp (const QList<QPointF> &points, const Data &data)
{ if (points.count () == 2)
  { QList<double> time = data.time ();
    QList<double> temperature = data.temperature ();

    QList<double> x;
    QList<double> y;

    for (int index = 0; index < time.length (); index++)
    { if (time.at (index) >= points.at (0).x ())
      { x.append (time.at (index));
        y.append (temperature.at (index));
      }

      if (time.at (index) >= points.at (1).x ()) break;
    }

    QList<double> coeffs = Regression::linear (x, y);
    return Ramp (coeffs.at (0), coeffs.at (1));
  }

  return Ramp ();
}

double Processor::startTime (const Data &data)
{ int index = startIndex (data.temperature ());
  QList<double> time = data.time ();
  return (index != -1?time.at (index):qSNaN ());
}

double Processor::peakTime (const Data &data)
{ int index = peakIndex (data.temperature ());
  QList<double> time = data.time ();
  return (index != -1?time.at (index):qSNaN ());
}

int Processor::startIndex (const QList<double> &values)
{ int index = 0;
  double sum = 0.0;
  int count = 0;

  for (; index < qMin (10, values.count ()); index++)
  { sum += values.at (index);
    count++;
  }

  for (; index < values.count (); index++)
  { double average = sum / static_cast<double> (count);
    if (values.at (index) > (1.1 * average)) break;

    sum += values.at (index);
    count++;
  }

  return (index < values.count ()?index:-1);
}

int Processor::peakIndex (const QList<double> &values)
{ double peakValue = -1.0e100;
  int peakIndex = -1;

  for (int index = 0; index < values.count (); index++)
    if (values.at (index) > peakValue)
    { peakValue = values.at (index);
      peakIndex = index;
    }

  return peakIndex;
}

double Processor::interp (const double x0, const double x1, const double y0, const double y1, const double x)
{ if (x0 == x1) return (y0 + y1) / 2.0;
  return ((y0 * (x1 - x)) + (y1 * (x - x0))) / (x1 - x0);
}

QPointF Processor::find (const double time, const Data &data)
{ QList<double> x = data.time ();
  QList<double> y = data.temperature ();

  int index;
  for (index = 0; (index < x.count ()) && (x.at (index) < time); index++);

  return QPointF (time, interp (x.at (index - 1), x.at (index), y.at (index - 1), y.at (index), time));
}
