#include <QFile>
#include <QTextStream>
#include <math.h>
#include <exception>
#include <boost/math/distributions/fisher_f.hpp>
#include "../../lib/math/regression.h"
#include "ct.h"

const int Ct::confirm = 9;                  // Number of additional cycles needed to confirm growth. Increased to 10 in order to detect low growth.
const double Ct::confirmRise = 1.05;        // Used to confirm low growth (after lowering minRise to handle inhibited profiles).
const double Ct::minRise = 1.1;             // Minimum percent fluorescence rise to consider profile positive. Inhibited nasal swab required adding further complexity.
const int Ct::k = 2;                        // Number of parameters in models: intercept plus slope.
const int Ct::initialCyclesInit = 5;

Ct::Ct (const PcrProfile &profile, const CtConfiguration &config) : m_id (profile.id ())
{ // Convert profile to milliVolts and apply gain offset.
  m_rawProfile = profile.transformed (config.rawProfileGain () * 1000.0, config.rawProfileOffset ());

  // Log-transform profile to linearize background and growth sections.
  QList<double> logProfile;
  for (int index = 0; index < m_rawProfile.count (); index++) logProfile.append (log (m_rawProfile.at (index)));

  m_breakpoint = 0;                       // Break point detected? If so, indicate in which cycle it was detected.
  int breakpointLowGrowth = 0;
  int initialCycles = initialCyclesInit;  // Initial number of cycles where growth cannot be detected. Becomes last cycle of background.
  int artifact = config.artifact ();      // Initial artifact detected? If so, indicate in how many cycles to avoid.
  //int inflection = logProfile.count ();   // Cycle where plateau process starts (i.e. inflection point).
  int startBackground = 1;                // By default, start background at cycle 1. It might change later if intial artifacts are found.
  int cycle = initialCycles + confirm;    // Need to have at least these many cycles to start testing for breakpoints.

  m_maxRise = 0.0;                        // Used to study limit for minRise.
  m_maxDeltaBeta = 0.0;                   // Used to study break point decision.
  double lowGrowth = 1.03;                // used to detect low growth on attenuated profiles. Used with ConfrmRise.
  double breakpointLowGrowthPval;

  // Test for break point at each cycle, using Chow Test (https://en.wikipedia.org/wiki/Chow_test).
  while ((cycle < logProfile.count ()) && !m_breakpoint)
  { if (artifact)
    { startBackground = artifact;
      initialCycles += artifact;
      cycle += artifact;
      artifact = 0;
    }

    // Calculate piece wise linear models for Chow test statistic.
    QList<double> backgroundSectionLm = fit (logProfile, startBackground - 1, initialCycles);
    QList<double> growthSectionLm = fit (logProfile, initialCycles - 1, cycle);
    QList<double> bothSectionLm = fit (logProfile, startBackground - 1, cycle);

    double s1 = ssr (logProfile, backgroundSectionLm, startBackground - 1, initialCycles);
    double s2 = ssr (logProfile, growthSectionLm, initialCycles - 1, cycle);
    double sCombined = ssr (logProfile, bothSectionLm, startBackground - 1, cycle);

    int lengthBackground = (initialCycles - startBackground) + 1;
    int lengthGrowth = (cycle - initialCycles) + 1;
    int totalDf = lengthBackground + lengthGrowth - 2 * k;
    double chowTestStatistic = ((sCombined - (s1 + s2)) / static_cast<double> (k)) / ((s1 + s2) / static_cast<double> (totalDf));

    //QTextStream (stdout) << "totalDf=" << totalDf << ", chowTs=" << chowTestStatistic << Qt::endl;

    boost::math::fisher_f dist (k, totalDf);
    double cdf = 0.0;
    try { cdf = boost::math::cdf (dist, chowTestStatistic); } catch (std::domain_error &e) {}

    m_breakpointPval = 1.0 - cdf;
    breakpointLowGrowthPval = qSNaN ();

    /* Ming: 09/25/23, to fix the SmoothNP graph issue. Change profile(V) to m_rawProfile(mV) for proper scalingo of
                       netProfile and SmoothNP calculation for the rest of the code in this routine */
    // Check for break point using p-value, minimum rise and increasing slope conditions.
    //double growth = profile.at (cycle - 1) / profile.at (initialCycles - 1);
    double growth = m_rawProfile.at (cycle - 1) / m_rawProfile.at (initialCycles - 1);
    if ((m_breakpointPval < 0.05) && (growth > minRise) && (growthSectionLm.at (1) > backgroundSectionLm.at (1)))
    { // Found breakpoint.
      m_breakpoint = initialCycles;
    } else
    { //Check for low growth, increment cycle counters and continue looping.
      if ((m_breakpointPval < 0.05) && (growth > lowGrowth))
      { // Found candidate for low growth.
        breakpointLowGrowth = initialCycles;
        breakpointLowGrowthPval = m_breakpointPval;

        // Update lowGrowth limit in case profile still growing.
        lowGrowth = growth;
      }

      initialCycles++;
      cycle++;
    }

    double rise = m_rawProfile.at (cycle - 1) / m_rawProfile.at (initialCycles - 1);
    if (rise > m_maxRise) m_maxRise = rise;

    double deltaBeta = growthSectionLm.at (1) - backgroundSectionLm.at (1);
    if (deltaBeta > m_maxDeltaBeta) m_maxDeltaBeta = deltaBeta;
  }

  // Check for low growth candidate confirmation, in case growth does not reach minRise.
  if (!m_breakpoint && breakpointLowGrowth)
  { if (((m_rawProfile.at (logProfile.count () - 1)) / m_rawProfile.at (breakpointLowGrowth - 1)) > confirmRise)
    { QTextStream (stdout) << "###################### LG ######################" << Qt::endl;

      m_breakpoint = breakpointLowGrowth;
      m_breakpointPval = breakpointLowGrowthPval;
      initialCycles = breakpointLowGrowth;
      //cycle = initialCycles + confirm;
    }
  }

  m_baselineAverage = average (m_rawProfile, initialCyclesInit, initialCycles);
  m_baselineStddev = stddev (m_rawProfile, initialCyclesInit, initialCycles);
  m_breakpoint = initialCycles;

  QList<double> backgroundHatX;
  QList<double> backgroundHatY;
  for (int index = startBackground - 1; index < initialCycles; index++)
  { backgroundHatX.append (index + 1);
    backgroundHatY.append (m_rawProfile.at (index));
  }
  QList<double> backgroundHatLm = Regression::linear (backgroundHatX, backgroundHatY);

  double maxBackground = 0.0;
  for (const double background : backgroundHatY) maxBackground = qMax (maxBackground, background);

  for (int index = 0; index < profile.numOfCycles (); index++)
    m_netProfile.append (m_rawProfile.at (index) - (backgroundHatLm.at (0) + (static_cast<double> (index + 1) * backgroundHatLm.at (1))));

  m_baselineNetProfileStddev = stddev (m_netProfile, initialCyclesInit, initialCycles);

  m_smoothNp.append ((m_netProfile.at (0) + m_netProfile.at (1)) / 2.0);
  for (int index = 1; index < (profile.numOfCycles () - 1); index++)
    m_smoothNp.append ((m_netProfile.at (index - 1) + m_netProfile.at (index) + m_netProfile.at (index + 1)) / 3.0);
  m_smoothNp.append ((m_netProfile.at (profile.numOfCycles () - 2) + m_netProfile.at (profile.numOfCycles () - 1)) / 2.0);

  double threshold = (config.autoThreshold ()?config.autoThresholdMultiplier () * m_baselineNetProfileStddev:config.threshold ());

  int index = profile.numOfCycles ();
  while ((m_smoothNp.at (index - 1) > threshold) && (index > 0)) index--;
  m_ctCount = ((index > 0) && (index < profile.numOfCycles ())?static_cast<double> (index) + ((threshold - m_smoothNp.at (index - 1)) / (m_smoothNp.at (index) - m_smoothNp.at (index - 1))):qSNaN ());
}

double Ct::ctCount (void) const
{ return m_ctCount;
}

int Ct::numOfCycles (void) const
{ return m_rawProfile.count ();
}

QList<double> Ct::rawProfile (void) const
{ return m_rawProfile;
}

QList<double> Ct::netProfile (void) const
{ return m_netProfile;
}

QList<double> Ct::smoothNp (void) const
{ return m_smoothNp;
}

double Ct::baselineAverage (void) const
{ return m_baselineAverage;
}

double Ct::baselineStddev (void) const
{ return m_baselineStddev;
}

double Ct::baselineNetProfileStddev (void) const
{ return m_baselineNetProfileStddev;
}

int Ct::breakpoint (void) const
{ return m_breakpoint;
}

double Ct::breakpointPval (void) const
{ return m_breakpointPval;
}

double Ct::maxRise (void) const
{ return m_maxRise;
}

double Ct::maxDeltaBeta (void) const
{ return m_maxDeltaBeta;
}

CuvetteId Ct::id (void) const
{ return m_id;
}

QList<Ct> Ct::process (const QList<PcrProfile> &profiles, const CtConfiguration &config)
{ QList<Ct> result;
  for (const PcrProfile &profile : profiles) result.append (Ct (profile, config));
  return result;
}

double Ct::ssr (const QList<double> &values, const QList<double> &coeffs, const int start, const int end)
{ double result = 0.0;

  for (int index = start; index < end; index++)
  { double residual = values.at (index) - (coeffs.at (0) + static_cast<double> (index + 1) * coeffs.at (1));
    result += (residual * residual);
  }

  return result;
}

QList<double> Ct::fit (const QList<double> &values, const int start, const int end)
{ QList<double> x;
  QList<double> y;

  for (int index = start; index < end; index++)
  { x.append (index + 1);
    y.append (values.at (index));
  }

  return Regression::linear (x, y);
}

double Ct::average (const QList<double> &values, const int start, const int end)
{ if (end <= start) return qSNaN ();

  double result = 0.0;
  for (int index = start; index < end; index++) result += values.at (index);

  return result / static_cast<double> (end - start);
}

double Ct::stddev (const QList<double> &values, const int start, const int end)
{ if (end <= start) return qSNaN ();

  double avg = average (values, start, end);
  double result = 0.0;

  for (int index = start; index < end; index++)
  { double temp = values.at (index) - avg;
    result += (temp * temp);
  }

  return sqrt (result / static_cast<double> (end - start));
}

bool Ct::dump (const QList<double> &profile, const QList<double> &logProfile, const QList<double> &netProfile, const QList<double> &smoothNp, const double ct)
{ QFile file ("dump.csv");
  if (!file.open (QFile::WriteOnly | QFile::Truncate | QFile::Text)) return false;

  for (int index = 0; index < profile.count (); index++)
  { QString line = QString::number (index + 1) + "\t" + QString::number (profile.at (index), 'e') + "\t" + QString::number (logProfile.at (index), 'e') + "\t" + QString::number (netProfile.at (index), 'e') + "\t" + QString::number (smoothNp.at (index), 'e') + "\t" + QString::number (ct, 'e') + "\n";
    file.write (line.toLatin1 ());
  }

  file.close ();

  return true;
}
