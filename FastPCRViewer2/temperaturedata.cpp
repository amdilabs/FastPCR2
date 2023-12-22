#include "temperaturedata.h"

TemperatureData::TemperatureData (void)
{
}

TemperatureData::TemperatureData (QIODevice *device)
{ char data [112];
  qint64 milliseconds = -1;

  for (;;)
  { if (device->read (data, 112) != 112) break;

    qint64 temp = toInt64 (data, 0);
    if (milliseconds < 0)
    { milliseconds = temp;
      m_timestamp = QDateTime::fromMSecsSinceEpoch (milliseconds);
    }
    m_time.append (static_cast<double> (temp - milliseconds) / 1000.0);

    m_top65Rtd1.append (toDouble (data, 8));
    m_top65Rtd2.append (toDouble (data, 16));
    m_top65Thermistor.append (toDouble (data, 24));
    m_bottom65Rtd1.append (toDouble (data, 32));
    m_bottom65Rtd2.append (toDouble (data, 40));
    m_bottom65Thermistor.append (toDouble (data, 48));
    m_top95Rtd1.append (toDouble (data, 56));
    m_top95Rtd2.append (toDouble (data, 64));
    m_top95Thermistor.append (toDouble (data, 72));
    m_bottom95Rtd1.append (toDouble (data, 80));
    m_bottom95Rtd2.append (toDouble (data, 88));
    m_bottom95Thermistor.append (toDouble (data, 96));
    m_heatSealer.append (toDouble (data, 104));
  }
}

QDateTime TemperatureData::timestamp (void) const
{ return m_timestamp;
}

QList<double> TemperatureData::time (void) const
{ return m_time;
}

QList<double> TemperatureData::top65Rtd1 (void) const
{ return m_top65Rtd1;
}

QList<double> TemperatureData::top65Rtd2 (void) const
{ return m_top65Rtd2;
}

QList<double> TemperatureData::top65Thermistor (void) const
{ return m_top65Thermistor;
}

QList<double> TemperatureData::bottom65Rtd1 (void) const
{ return m_bottom65Rtd1;
}

QList<double> TemperatureData::bottom65Rtd2 (void) const
{ return m_bottom65Rtd2;
}

QList<double> TemperatureData::bottom65Thermistor (void) const
{ return m_bottom65Thermistor;
}

QList<double> TemperatureData::top95Rtd1 (void) const
{ return m_top95Rtd1;
}

QList<double> TemperatureData::top95Rtd2 (void) const
{ return m_top95Rtd2;
}

QList<double> TemperatureData::top95Thermistor (void) const
{ return m_top95Thermistor;
}

QList<double> TemperatureData::bottom95Rtd1 (void) const
{ return m_bottom95Rtd1;
}

QList<double> TemperatureData::bottom95Rtd2 (void) const
{ return m_bottom95Rtd2;
}

QList<double> TemperatureData::bottom95Thermistor (void) const
{ return m_bottom95Thermistor;
}

QList<double> TemperatureData::heatSealer (void) const
{ return m_heatSealer;
}

qint64 TemperatureData::toInt64 (char *data, const int offset)
{ qint64 result;
  memcpy (&result, &data [offset], sizeof (qint64));
  return result;
}

double TemperatureData::toDouble (char *data, const int offset)
{ double result;
  memcpy (&result, &data [offset], sizeof (double));
  return result;
}
