#include <QFile>
#include <QJsonParseError>
#include "../../lib/inflate.h"
#include "../io.h"
#include "../tagreader.h"
#include "../tags.h"
#include "filereader.h"

CaptureConfiguration FileReader::readCaptureConfig (const QString &fileName)
{ return CaptureConfiguration (readJson (fileName, Tags::captureConfiguration).array ());
}

CuvetteLocationConfigurationContainer FileReader::readCuvetteLocationConfig (const QString &fileName)
{ return CuvetteLocationConfigurationContainer (readJson (fileName, Tags::cuvetteLocationConfiguration).array ());
}

QList<RawData> FileReader::readRawData (const QString &fileName)
{ QList<RawData> result;

  QFile file (fileName);
  if (file.open (QFile::ReadOnly))
  { QIODevice *device = init (&file);
    if (device != nullptr)
    { TagReader reader (device, Tags::rawData);
      if (reader.open (TagReader::ReadOnly))
      { for (;;)
        { RawData data (&reader);

          if (!data.isValid ()) break;
          result.append (data);
        }

        reader.close ();
      }

      device->close ();
      delete device;
    }

    file.close ();
  }

  return result;
}

TemperatureData FileReader::readTemperatures (const QString &fileName)
{ TemperatureData result;

  QFile file (fileName);
  if (file.open (QFile::ReadOnly))
  { QIODevice *device = init (&file);
    if (device != nullptr)
    { TagReader reader (device, Tags::temperatures);
      if (reader.open (TagReader::ReadOnly))
      { result = TemperatureData (&reader);
        reader.close ();
      }

      device->close ();
      delete device;
    }

    file.close ();
  }

  return result;
}

QList<QPair<QString, QString> > FileReader::readUserInput (const QString &fileName)
{ QJsonArray array = readJson (fileName, Tags::userInput).array ();

  QList<QPair<QString, QString> > result;
  for (const QJsonValue &value : array)
  { QJsonObject object = value.toObject ();
    result.append (QPair<QString, QString> (object.value ("key").toString (), jsonValueToString (object.value ("value")) + object.value ("unit").toString ()));
  }

  return result;
}

QIODevice *FileReader::init (QIODevice *device)
{ if (IO::ri32 (device) != magicNumber) return nullptr;
  if (IO::ri32 (device) != version) return nullptr;

  Inflate *inflate = new Inflate (device);
  if (!inflate->open (Inflate::ReadOnly))
  { delete inflate;
    return nullptr;
  }

  return inflate;
}

QJsonDocument FileReader::readJson (const QString &fileName, const int tag)
{ QJsonDocument result;

  QFile file (fileName);
  if (file.open (QFile::ReadOnly))
  { QIODevice *device = init (&file);
    if (device != nullptr)
    { TagReader reader (device, tag);
      if (reader.open (TagReader::ReadOnly))
      { QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson (reader.readAll (), &error);
        reader.close ();

        if (error.error == QJsonParseError::NoError) result = doc;
      }

      device->close ();
      delete device;
    }

    file.close ();
  }

  return result;
}

QString FileReader::jsonValueToString (const QJsonValue &value)
{ if (value.isDouble ()) return QString::number (value.toDouble ());
  return value.toString ();
}
