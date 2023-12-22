#ifndef PCRPROFILEPLOT_H
#define PCRPROFILEPLOT_H

#include "../../lib/ui/customqcustomplot.h"
#include "../optics/pcrprofile.h"

class PcrProfilePlot : public CustomQCustomPlot
{ Q_OBJECT

  public:
    explicit PcrProfilePlot (QWidget *parent = nullptr);

    void zoomAll (void) override;

    int channelMask (void) const;
    int cuvetteMask (void) const;

    void setChannelMask (const int channelMask);
    void setCuvetteMask (const int cuvetteMask);

  public slots:
    void plot (const QList<PcrProfile> &profiles);

  private:
    int m_channelMask;
    int m_cuvetteMask;
    int m_maxNumOfCycles;
    QList<PcrProfile> m_profiles;

    void addGraph (const PcrProfile &profile, const QColor &colour, const QCPScatterStyle::ScatterShape shape);
    static QList<double> x (const int numOfCycles);
    static QList<double> y (const QList<double> &values);
};

#endif // PCRPROFILEPLOT_H
