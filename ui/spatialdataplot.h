#ifndef SPATIALDATAPLOT_H
#define SPATIALDATAPLOT_H

#include "../../lib/ui/customqcustomplot.h"
#include "../optics/cuvettelocationconfigurationcontainer.h"
#include "../optics/spatialdata.h"

class SpatialDataPlot : public CustomQCustomPlot
{ Q_OBJECT

  public:
    explicit SpatialDataPlot (QWidget *parent = nullptr);

    void zoomAll (void) override;

    int channelMask (void) const;
    void setChannelMask (const int channelMask);

    CuvetteLocationConfigurationContainer cuvetteConfig (void) const;
    void setCuvetteConfig (const CuvetteLocationConfigurationContainer &config);

  public slots:
    void plot (const QList<SpatialData> &data);

  private:
    int m_channelMask;
    CuvetteLocationConfigurationContainer m_cuvetteConfig;
    double m_positionMin;
    double m_positionMax;

    QList<SpatialData> m_data;

    static QList<double> convertAdc (const QList<double> &list);
};

#endif // SPATIALDATAPLOT_H
