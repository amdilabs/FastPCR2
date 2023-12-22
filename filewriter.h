#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QFile>
#include "../../lib/log.h"
#include "../../lib/ui/userinput/userinputdata.h"
#include "../lib/deflate.h"
#include "cycler/pcrprotocol.h"
#include "cycler/pge.h"
#include "optics/captureconfiguration.h"
#include "optics/cuvettelocationconfigurationcontainer.h"
#include "optics/rawdata.h"
#include "tagwriter.h"

class FileWriter
{ public:
    FileWriter (void);
    ~FileWriter (void);

    bool open (const QString &fileName);
    void close (void);

    bool writeCaptureConfiguration (const CaptureConfiguration &config);
    bool writeCuvetteLocationConfiguration (const CuvetteLocationConfigurationContainer &config);
    bool writeEncoder (const QByteArray &data);
    bool writeGripperConfiguration (const Pge::Select select, const Pge::IoGroup ioGroup, const double position, const double force, const double speed);
    bool writeLog (const QDateTime &timestamp, const Log::Severity severity, const QString &source, const QString &text);
    bool writePcrProtocol (const PcrProtocol &protocol);
    bool writeRawData (const RawData &data);
    bool writeTemperatures (const QDateTime &timestamp, const QList<double> &top65, const QList<double> &bottom65, const QList<double> &top95, const QList<double> &bottom95, const double heatSealer);
    bool writeUserInput (const UserInputData &data);

  private:
    static const int magicNumber = 0x52435066;
    static const int version = 1;

    QFile *m_file;
    Deflate *m_deflate;
    QMap<int, TagWriter *> m_writers;

    TagWriter *writer (const int tag);
    bool writeJson (const int tag, const QJsonDocument &doc);
};

#endif // FILEWRITER_H
