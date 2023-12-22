#include <QCommandLineParser>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include "../../lib/inflate.h"
#include "../io.h"

static const int magicNumber = 0x52435066;
static const int version = 1;

QString tagToString (const int tag)
{ QByteArray ba;
  ba.append (tag);
  ba.append (tag >> 8);
  ba.append (tag >> 16);
  ba.append (tag >> 24);
  return QString::fromLatin1 (ba);
}

void dump (const QString &fileName)
{ QFile in (fileName);
  if (in.open (QFile::ReadOnly))
  { Inflate i (&in);
    if (i.open (Inflate::ReadOnly))
    { if (IO::ri32 (&in) == magicNumber)
      { if (IO::ri32 (&in) == version)
        { QMap<int, QFile *> files;

          for (;;)
          { int tag = IO::ri32 (&i);
            if (tag == -1) break;

            int size = IO::ri32 (&i);

            if (!files.contains (tag))
            { QFileInfo info (fileName);
              QString newFileName = info.absolutePath () + QDir::separator () + info.completeBaseName () + "." + tagToString (tag);
              QFile *file = new QFile (newFileName);
              if (file->open (QFile::WriteOnly | QFile::Truncate))
              { files.insert (tag, file);
              } else
              { delete file;
              }
            }

            QFile *file = files.value (tag);
            if (file != nullptr) file->write (i.read (size));
          }

          QList<QFile *> temp = files.values ();
          for (QFile *file : temp)
          { file->close ();
            delete file;
          }

        }
      }

      i.close ();
    }

    in.close ();
  }
}

int main (int argc, char *argv[])
{ QCoreApplication a (argc, argv);

  QCommandLineParser parser;
  parser.addPositionalArgument ("file", "File name");
  parser.process (a);

  QStringList args = parser.positionalArguments ();
  for (const QString &fileName : args) dump (fileName);

  return 0;
}
