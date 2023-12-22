#ifndef TEMPERATUREPLOT_H
#define TEMPERATUREPLOT_H

#include "../../lib/ui/customqcustomplot.h"

class TemperaturePlot : public CustomQCustomPlot
{ Q_OBJECT

  public:
    explicit TemperaturePlot (QWidget *parent = nullptr);
    ~TemperaturePlot (void);

    void zoomAll (void) override;

  public slots:
    void temperaturesReceived (const QList<double> &top65, const QList<double> &bottom65, const QList<double> &top95, const QList<double> &bottom95, const double heatSealer);

  private:
    static const int size;
    static const QColor colours [];
    static const int numOfColours;

    QCPGraph *m_graphTop65 [3];
    QCPGraph *m_graphBottom65 [3];
    QCPGraph *m_graphTop95 [3];
    QCPGraph *m_graphBottom95 [3];
    QCPGraph *m_graphHeatSealer;

    int m_index;
    int m_used;
    double *m_top65 [3];
    double *m_bottom65 [3];
    double *m_top95 [3];
    double *m_bottom95 [3];
    double *m_heatSealer;

    QVector<double> m_x;

    QColor getColour (void);
    QVector<double> vec (double *data) const;
};

#endif // TEMPERATUREPLOT_H
