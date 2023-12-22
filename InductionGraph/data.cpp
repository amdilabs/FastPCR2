#include <QFile>
#include <QFileInfo>
#include "data.h"

Data::Data (void) : m_valid (false)
{
}

Data::Data (const QString &fileName) : m_valid (false)
{ QFileInfo info (fileName);
  m_name = info.baseName ();

  QFile file (fileName);
  if (file.open (QFile::ReadOnly | QFile::Text))
  { // Skip header.
    file.readLine ();

    bool first = true;
    bool ok = false;

    for (;;)
    { QStringList columns = QString::fromUtf8 (file.readLine ()).split (';');
      if (columns.count () != 2) break;

      int days = columns.at (0).mid (0, 2).toInt (&ok); if (!ok) break;
      int months = columns.at (0).mid (3, 2).toInt (&ok); if (!ok) break;
      int years = columns.at (0).mid (6, 4).toInt (&ok); if (!ok) break;
      int hours = columns.at (0).mid (11, 2).toInt (&ok); if (!ok) break;
      int minutes = columns.at (0).mid (14, 2).toInt (&ok); if (!ok) break;
      int seconds = columns.at (0).mid (17, 2).toInt (&ok); if (!ok) break;
      int milliseconds = columns.at (0).mid (20, 2).toInt (&ok) * 10; if (!ok) break;
      QDateTime timestamp (QDate (years, months, days), QTime (hours, minutes, seconds, milliseconds));

      double temperature = columns.at (1).trimmed ().toDouble (&ok); if (!ok) break;

      if (first)
      { first = false;
        m_timestamp = timestamp;
      }

      m_time.append (static_cast<double> (m_timestamp.msecsTo (timestamp)) / 1000.0);
      m_temperature.append (temperature);
    }

    m_valid = ok;

    file.close ();
  }
}

bool Data::isValid (void) const
{ return m_valid;
}

QString Data::name (void) const
{ return m_name;
}

QDateTime Data::timestamp (void) const
{ return m_timestamp;
}

QList<double> Data::time (void) const
{ return m_time;
}

QList<double> Data::time (const double offset) const
{ QList<double> result; result.reserve (m_time.count ());
  for (const double value : m_time) result.append (value - offset);
  return result;
}

QList<double> Data::temperature (void) const
{ return m_temperature;
}
