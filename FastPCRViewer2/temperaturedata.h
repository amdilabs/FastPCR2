#ifndef TEMPERATUREDATA_H
#define TEMPERATUREDATA_H

#include <QDateTime>
#include <QIODevice>
#include <QList>

class TemperatureData
{ public:
    TemperatureData (void);
    TemperatureData (QIODevice *device);

    QDateTime timestamp (void) const;

    QList<double> time (void) const;

    QList<double> top65Rtd1 (void) const;
    QList<double> top65Rtd2 (void) const;
    QList<double> top65Thermistor (void) const;

    QList<double> bottom65Rtd1 (void) const;
    QList<double> bottom65Rtd2 (void) const;
    QList<double> bottom65Thermistor (void) const;

    QList<double> top95Rtd1 (void) const;
    QList<double> top95Rtd2 (void) const;
    QList<double> top95Thermistor (void) const;

    QList<double> bottom95Rtd1 (void) const;
    QList<double> bottom95Rtd2 (void) const;
    QList<double> bottom95Thermistor (void) const;

    QList<double> heatSealer (void) const;

  private:
    QDateTime m_timestamp;
    QList<double> m_time;
    QList<double> m_top65Rtd1;
    QList<double> m_top65Rtd2;
    QList<double> m_top65Thermistor;
    QList<double> m_bottom65Rtd1;
    QList<double> m_bottom65Rtd2;
    QList<double> m_bottom65Thermistor;
    QList<double> m_top95Rtd1;
    QList<double> m_top95Rtd2;
    QList<double> m_top95Thermistor;
    QList<double> m_bottom95Rtd1;
    QList<double> m_bottom95Rtd2;
    QList<double> m_bottom95Thermistor;
    QList<double> m_heatSealer;

    static qint64 toInt64 (char *data, const int offset);
    static double toDouble (char *data, const int offset);
};

#endif // TEMPERATUREDATA_H
