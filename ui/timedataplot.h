#ifndef TIMEDATAPLOT_H
#define TIMEDATAPLOT_H

#include "../../lib/ui/customqcustomplot.h"
#include "../optics/timedata.h"

class TimeDataPlot : public CustomQCustomPlot
{ Q_OBJECT

  public:
    explicit TimeDataPlot (QWidget *parent = nullptr);

    void zoomAll (void) override;

    int channelMask (void) const;
    void setChannelMask (const int channelMask);

  public slots:
    void plot (const QList<TimeData> &data);

  private:
    int m_channelMask;
    double m_time;
    int m_encoderMin;
    int m_encoderMax;

    QList<TimeData> m_data;

    static QList<double> time (const int numOfSamples);
    static QList<double> convertEncoder (const QList<int> &list, int *min, int *max);
    static QList<double> convertAdc (const QList<double> &list);
};

#endif // TIMEDATAPLOT_H
