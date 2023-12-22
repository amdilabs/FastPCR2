#include "../../lib/math/regression.h"
#include "cuvetteintensities.h"

CuvetteIntensities::CuvetteIntensities (void) : m_timestamp (0), m_channel (-1)
{
}

CuvetteIntensities::CuvetteIntensities (const SpatialData &data, const CuvetteLocationConfiguration &locationConfig, const CuvetteProcessingConfiguration &processingConfig) : m_timestamp (data.timestamp ()), m_channel (data.channel ())
{ QList<double> adc = data.adc ();

  if (processingConfig.biasCorrect ())
  { // Apply bias correction.
    adc = subtract (adc, bias (adc, data.position (), locationConfig.darkStart (), locationConfig.darkEnd ()));
  }

  // Process cuvettes.
  for (int cuvette = 0; cuvette < locationConfig.numOfCuvettes (); cuvette++)
  { double start = locationConfig.cuvetteStart (cuvette);
    double end = locationConfig.cuvetteEnd (cuvette);

    if (processingConfig.autoCuvetteOffsetAdjust ())
    { // Set center of cuvette to where peak is found.
      double location = findPeak (adc, data.position (), start, end);
      start = location - 0.5 * locationConfig.cuvetteWidth ();
      end = location + 0.5 * locationConfig.cuvetteWidth ();
    }

    if (processingConfig.algorithm () == CuvetteProcessingConfiguration::GaussianCurveFit)
    { // Apply Gaussian curve fitting to cuvettes.
      Gaussian g = gaussian (adc, data.position (), start, end);
      m_gaussian.append (g);
      m_values.append (g.a () < 10.0?g.a ():qSNaN ());
      m_locations.append (g.mu ());
    } else
    if (processingConfig.algorithm () == CuvetteProcessingConfiguration::Integration)
    { m_values.append (integrate (adc, data.position (), start, end) / locationConfig.cuvetteWidth ());
      m_locations.append (static_cast<double> (start + end) * 0.5);
    } else
    if (processingConfig.algorithm () == CuvetteProcessingConfiguration::PeakAmplitude)
    { double amplitude;
      m_locations.append (findPeak (adc, data.position (), start, end, &amplitude));
      m_values.append (amplitude);
    }

   // QTextStream (stdout) << "ch=" << channel << ", cuv=" << cuvette << ", pl=" << peakLoc << ", pa=" << peakAmp << ", gu=" << g.mu () << ", ga=" << g.a () << Qt::endl;
  }
}

int CuvetteIntensities::timestamp (void) const
{ return m_timestamp;
}

int CuvetteIntensities::numOfCuvettes (void) const
{ return m_values.count ();
}

int CuvetteIntensities::channel (void) const
{ return m_channel;
}

QList<double> CuvetteIntensities::values (void) const
{ return m_values;
}

QList<Gaussian> CuvetteIntensities::gaussian (void) const
{ return m_gaussian;
}

QList<double> CuvetteIntensities::locations (void) const
{ return m_locations;
}

QList<double> CuvetteIntensities::subtract (const QList<double> &list, const double value)
{ QList<double> result;
  result.reserve (list.count ());

  for (const double l : list) result.append (l - value);

  return result;
}

double CuvetteIntensities::bias (const QList<double> &adc, const QList<double> &position, const double start, const double end)
{ double result = 0.0;
  int count = 0;

  for (int index = 0; index < position.count (); index++)
  { double pos = position.at (index);
    if ((pos >= start) && (pos < end))
    { result += adc.at (index);
      count++;
    }
  }

  return (count > 0?result / static_cast<double> (count):qSNaN ());
}

Gaussian CuvetteIntensities::gaussian (const QList<double> &adc, const QList<double> &position, const double start, const double end)
{ QList<double> x;
  QList<double> y;

  for (int index = 0; index < position.count (); index++)
  { double pos = position.at (index);
    if ((pos >= start) && (pos < end))
    { x.append (pos);
      y.append (log (adc.at (index)));
    }
  }

  QList<double> coeffs = Regression::polynomial (x, y, 2);

  double mu = -coeffs.at (1) / (2.0 * coeffs.at (2));
  double sigma = sqrt (-1.0 / (2.0 * coeffs.at (2)));
  double a = exp (coeffs.at (0) - ((coeffs.at (1) * coeffs.at (1)) / (4.0 * coeffs.at (2))));

  return Gaussian (mu, sigma, a);
}

double CuvetteIntensities::findPeak (const QList<double> &adc, const QList<double> &position, const double start, const double end, double *amplitude)
{ double result = start;
  double max = -1.0e100;

  for (int index = 0; index < position.count (); index++)
  { double pos = position.at (index);
    if ((pos >= start) && (pos < end))
    { if (adc.at (index) > max)
      { result = pos;
        max = adc.at (index);
      }
    }
  }

  if (amplitude) *amplitude = (max >= 0.0?max:qSNaN ());
  return result;
}

double CuvetteIntensities::integrate (const QList<double> &adc, const QList<double> &position, const double start, const double end)
{ double result = 0.0;

  if (!position.isEmpty ())
  { int index;
    for (index = 0; (index < position.count ()) && (start >= position.at (index)); index++);

    if (index < (position.count () - 1))
    { double x0 = position.at (index);
      double x1 = position.at (index + 1);
      double y0 = adc.at (index);
      double y1 = adc.at (index + 1);

      result = area (x0, x1, y0, y1, start, x1);
      index++;

      for (; (index < (position.count () - 1)) && (end > position.at (index)); index++)
      { x0 = x1;
        y0 = y1;

        x1 = position.at (index + 1);
        y1 = adc.at (index + 1);
        result += area (x0, x1, y0, y1, x0, x1);
      }

      index++;
      if (index < position.count ()) result += area (x1, position.at (index), y1, adc.at (index), x1, end);
    }
  }

  return result;
}

double CuvetteIntensities::interp (const double x0, const double x1, const double y0, const double y1, const double x)
{ return ((y0 * (x1 - x)) + (y1 * (x - x0))) / (x1 - x0);
}

double CuvetteIntensities::area (const double x0, const double x1, const double y0, const double y1, const double start, const double end)
{ double yA = (x0 == start?y0:interp (x0, x1, y0, y1, start));
  double yB = (x1 == end?y1:interp (x0, x1, y0, y1, end));
  double width = static_cast<double> (end - start);
  return width * qMin (yA, yB) + (0.5 * qAbs (yB - yA) * width);
}
