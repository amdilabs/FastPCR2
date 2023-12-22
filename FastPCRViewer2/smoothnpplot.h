#ifndef SMOOTHNPPLOT_H
#define SMOOTHNPPLOT_H

#ifdef STATIC
#include "../../lib/ui/customqcustomplotstatic.h"
#else
#include "../../lib/ui/customqcustomplot.h"
#endif
#include "../optics/ct.h"

class SmoothNpPlot : public CustomQCustomPlot
{ Q_OBJECT

  public:
    explicit SmoothNpPlot (QWidget *parent = nullptr);

    void zoomAll (void) override;

    int channelMask (void) const;
    int cuvetteMask (void) const;

    void setChannelMask (const int channelMask);
    void setCuvetteMask (const int cuvetteMask);

    void plot (const QList<Ct> &cts);
    void setThreshold (const double threshold);

  private:
    static const int minNumOfCycles;

    int m_channelMask;
    int m_cuvetteMask;
    int m_numOfCycles;
    QCPItemStraightLine *m_line;
    QList<Ct> m_cts;
};

#endif // SMOOTHNPPLOT_H
