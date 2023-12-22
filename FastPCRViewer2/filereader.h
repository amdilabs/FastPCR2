#ifndef FILEREADER_H
#define FILEREADER_H

#include "../optics/captureconfiguration.h"
#include "../optics/cuvettelocationconfigurationcontainer.h"
#include "../optics/rawdata.h"
#include "temperaturedata.h"

class FileReader
{ public:
    static CaptureConfiguration readCaptureConfig (const QString &fileName);
    static CuvetteLocationConfigurationContainer readCuvetteLocationConfig (const QString &fileName);
    static QList<RawData> readRawData (const QString &fileName);
    static TemperatureData readTemperatures (const QString &fileName);
    static QList<QPair<QString, QString> > readUserInput (const QString &fileName);

  private:
    static const int magicNumber = 0x52435066;
    static const int version = 1;

    static QIODevice *init (QIODevice *device);
    static QJsonDocument readJson (const QString &fileName, const int tag);
    static QString jsonValueToString (const QJsonValue &value);
};

#endif // FILEREADER_H
