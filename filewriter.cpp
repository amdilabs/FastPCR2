#include <QJsonDocument>
#include "io.h"
#include "tags.h"
#include "filewriter.h"

FileWriter::FileWriter (void) : m_file (nullptr), m_deflate (nullptr)
{
}

FileWriter::~FileWriter (void)
{ close ();
}

bool FileWriter::open (const QString &fileName)
{ close ();

  m_file = new QFile (fileName);
  if (!m_file->open (QFile::WriteOnly | QFile::Truncate)) { delete m_file; m_file = nullptr; return false; }

  if (!IO::wi32 (m_file, magicNumber)) {  m_file->close (); delete m_file; m_file = nullptr; return false; }
  if (!IO::wi32 (m_file, version)) {  m_file->close (); delete m_file; m_file = nullptr; return false; }

  m_deflate = new Deflate (m_file, 9);
  if (!m_deflate->open (QFile::WriteOnly | QFile::Truncate)) { delete m_deflate; m_file->close (); delete m_file; m_file = nullptr; return false; }

  return true;
}

void FileWriter::close (void)
{ if (m_file != nullptr)
  { QList<int> tags = m_writers.keys ();
    for (const int tag : tags)
    { TagWriter *writer = m_writers.value (tag);
      writer->close ();
      delete writer;
    }
    m_writers.clear ();

    m_deflate->close ();
    m_file->close ();

    delete m_deflate;
    delete m_file;

    m_deflate = nullptr;
    m_file = nullptr;
  }
}

bool FileWriter::writeCaptureConfiguration (const CaptureConfiguration &config)
{ return writeJson (Tags::captureConfiguration, QJsonDocument (config.toJson ()));
}

bool FileWriter::writeCuvetteLocationConfiguration (const CuvetteLocationConfigurationContainer &config)
{ return writeJson (Tags::cuvetteLocationConfiguration, QJsonDocument (config.toJson ()));
}

bool FileWriter::writeEncoder (const QByteArray &data)
{ if (!m_file) return false;
  TagWriter *w = writer (Tags::encoder);

  if (w->write (data) != data.length ()) return false;

  return true;
}

bool FileWriter::writeGripperConfiguration (const Pge::Select select, const Pge::IoGroup ioGroup, const double position, const double force, const double speed)
{ if (!m_file) return false;
  TagWriter *w = writer (Tags::gripperConfiguration);

  if (w->write (reinterpret_cast<const char *> (&select), 4) != 4) return false;
  if (w->write (reinterpret_cast<const char *> (&ioGroup), 4) != 4) return false;
  if (w->write (reinterpret_cast<const char *> (&position), 8) != 8) return false;
  if (w->write (reinterpret_cast<const char *> (&force), 8) != 8) return false;
  if (w->write (reinterpret_cast<const char *> (&speed), 8) != 8) return false;

  return true;
}

bool FileWriter::writeLog (const QDateTime &timestamp, const Log::Severity severity, const QString &source, const QString &text)
{ if (!m_file) return false;

  QString sev;
  switch (severity)
  { case Log::Debug: sev = "[**]"; break;
    case Log::Info: sev = "[II]"; break;
    case Log::Warning: sev = "[WW]"; break;
    case Log::Error: sev = "[EE]"; break;
    default: sev = "[??]";
  }
  QString line = timestamp.toString ("yyyy-MM-ddThh:mm:ss.zzz") + " " + sev + " " + source + ": " + text + "\n";
  QByteArray data = line.toUtf8 ();

  TagWriter *w = writer (Tags::log);
  if (w->write (data) != data.length ()) return false;

  return true;
}

bool FileWriter::writePcrProtocol (const PcrProtocol &protocol)
{ return writeJson (Tags::pcrProtocol, QJsonDocument (protocol.toJson ()));
}

bool FileWriter::writeRawData (const RawData &data)
{ if (!m_file) return false;
  return data.write (writer (Tags::rawData));
}

bool FileWriter::writeTemperatures (const QDateTime &timestamp, const QList<double> &top65, const QList<double> &bottom65, const QList<double> &top95, const QList<double> &bottom95, const double heatSealer)
{ if (!m_file) return false;
  TagWriter *w = writer (Tags::temperatures);

  qint64 milliseconds = timestamp.toMSecsSinceEpoch ();
  if (w->write (reinterpret_cast<char *> (&milliseconds), 8) != 8) return false;
  if (w->write (reinterpret_cast<const char *> (top65.constData ()), 3 * 8) != (3 * 8)) return false;
  if (w->write (reinterpret_cast<const char *> (bottom65.constData ()), 3 * 8) != (3 * 8)) return false;
  if (w->write (reinterpret_cast<const char *> (top95.constData ()), 3 * 8) != (3 * 8)) return false;
  if (w->write (reinterpret_cast<const char *> (bottom95.constData ()), 3 * 8) != (3 * 8)) return false;
  if (w->write (reinterpret_cast<const char *> (&heatSealer), 8) != 8) return false;

  return true;
}

bool FileWriter::writeUserInput (const UserInputData &data)
{ return writeJson (Tags::userInput, QJsonDocument (data.toJson ()));
}

TagWriter *FileWriter::writer (const int tag)
{ if (m_writers.contains (tag)) return m_writers.value (tag);

  TagWriter *writer = new TagWriter (m_deflate, tag);
  writer->open (TagWriter::WriteOnly | TagWriter::Truncate);
  m_writers.insert (tag, writer);

  return writer;
}

bool FileWriter::writeJson (const int tag, const QJsonDocument &doc)
{ if (!m_file) return false;
  QByteArray data = doc.toJson (QJsonDocument::Compact);

  TagWriter *w = writer (tag);
  if (w->write (data) != data.size ()) return false;

  return w->flush ();
}
