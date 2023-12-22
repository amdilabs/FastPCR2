#ifndef CT_H
#define CT_H

#include "ctconfiguration.h"
#include "pcrprofile.h"

class Ct
{ public:
    Ct (const PcrProfile &profile, const CtConfiguration &config);

    double ctCount (void) const;
    int numOfCycles (void) const;
    QList<double> rawProfile (void) const;
    QList<double> netProfile (void) const;
    QList<double> smoothNp (void) const;
    double baselineAverage (void) const;
    double baselineStddev (void) const;
    double baselineNetProfileStddev (void) const;
    int breakpoint (void) const;
    double breakpointPval (void) const;
    double maxRise (void) const;
    double maxDeltaBeta (void) const;
    CuvetteId id (void) const;

    static QList<Ct> process (const QList<PcrProfile> &profiles, const CtConfiguration &config);

  private:
    static const int confirm;
    static const double confirmRise;
    static const double minRise;
    static const int k;
    static const int initialCyclesInit;

    double m_ctCount;
    QList<double> m_rawProfile;
    QList<double> m_netProfile;
    QList<double> m_smoothNp;
    double m_baselineAverage;
    double m_baselineStddev;
    double m_baselineNetProfileStddev;
    int m_breakpoint;
    double m_breakpointPval;
    double m_maxRise;
    double m_maxDeltaBeta;
    CuvetteId m_id;

    static double ssr (const QList<double> &values, const QList<double> &coeffs, const int start, const int end);
    static QList<double> fit (const QList<double> &values, const int start, const int end);
    static double average (const QList<double> &values, const int start, const int end);
    static double stddev (const QList<double> &values, const int start, const int end);

    static bool dump (const QList<double> &profile, const QList<double> &logProfile, const QList<double> &netProfile, const QList<double> &smoothNp, const double ct);
};

#endif // CT_H
