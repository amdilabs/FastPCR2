#ifndef PLOT_H
#define PLOT_H

#include "../../lib/ui/customqcustomplotstatic.h"
#include "../ui/colourmap.h"
#include "data.h"
#include "ramp.h"

class Plot : public CustomQCustomPlot
{ Q_OBJECT

  public:
    explicit Plot (QWidget *parent = nullptr);

    void zoomAll (void) override;
    void clear (void);
    void clearLast (void);
    void plot (const Data &data, const double offset, const Ramp &ramp);

  private:
    ColourMap m_map;
    double m_timeMin;
    double m_timeMax;
    double m_temperatureMin;
    double m_temperatureMax;

    QList<QCPGraph *> m_graphs;
    QList<QCPItemStraightLine *> m_lines;
};

#endif // PLOT_H
