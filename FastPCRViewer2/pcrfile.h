#ifndef PCRFILE_H
#define PCRFILE_H

#include <QObject>
#include "../optics/captureconfiguration.h"
#include "../optics/cuvettelocationconfigurationcontainer.h"
#include "../optics/rawdata.h"
#include "../optics/spatialdata.h"
#include "../optics/timedata.h"
#include "temperaturedata.h"

class PcrFile : public QObject
{ Q_OBJECT

  public:
    explicit PcrFile (QObject *parent = nullptr);

    void setCuvetteConfiguration (const CuvetteLocationConfigurationContainer &config);

    bool load (const QString &fileName);

    int numOfCycles (void) const;

    CaptureConfiguration captureConfig (void) const;
    CuvetteLocationConfigurationContainer cuvetteLocationConfig (void) const;
    RawData rawData (const int cycle) const;
    TemperatureData temperatures (void) const;
    QList<TimeData> timeData (const int cycle) const;
    QList<SpatialData> spatialData (const int cycle) const;
    QList<QPair<QString, QString> > userInput (void) const;

  signals:

  private:
    CaptureConfiguration m_captureConfig;
    CuvetteLocationConfigurationContainer m_cuvetteConfig;
    QList<RawData> m_rawData;
    QList<QList<TimeData> > m_timeData;
    QList<QList<SpatialData> > m_spatialData;
    QList<QPair<QString, QString> > m_userInput;
    TemperatureData m_temperatures;

    void process (void);
};

#endif // PCRFILE_H
