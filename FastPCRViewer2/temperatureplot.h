#ifndef TEMPERATUREPLOT_H
#define TEMPERATUREPLOT_H

#include "../../lib/ui/customqcustomplot.h"
#include "temperaturedata.h"

class TemperaturePlot : public CustomQCustomPlot
{ Q_OBJECT

  public:
    explicit TemperaturePlot (QWidget *parent = nullptr);

    void zoomAll (void) override;
    void plot (const TemperatureData &data);

  private:
    double m_timeMin;
    double m_timeMax;

    void addGraph (const QColor &colour, const QString &name, const QList<double> &x, const QList<double> &y);
};

#endif // TEMPERATUREPLOT_H
