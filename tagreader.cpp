#include <QTextStream>
#include "io.h"
#include "tagreader.h"

TagReader::TagReader (QIODevice *device, const int tag) : m_device (device), m_tag (tag), m_remain (-1)
{
}

TagReader::~TagReader (void)
{ close ();
}

bool TagReader::open (OpenMode mode)
{ if (!QIODevice::open (mode)) return false;

  m_remain = 0;

  return true;
}

void TagReader::close (void)
{ QIODevice::close ();
}

bool TagReader::isSequential (void) const
{ return true;
}

qint64 TagReader::readData (char *data, qint64 maxSize)
{ if (m_remain < 0) return 0;

  qint64 bytesRead = 0;

  while (maxSize)
  { if (!m_remain)
    { m_remain = next ();
      if (m_remain <= 0) break;
    }

    int amount = qMin (m_remain, maxSize);

    int read = m_device->read (data, amount);
    bytesRead += read;

    if (read != amount) break;

    m_remain -= amount;
    maxSize -= amount;
    data += amount;
  }

  return bytesRead;
}

qint64 TagReader::writeData (const char *data, qint64 maxSize)
{ Q_UNUSED(data)
  Q_UNUSED(maxSize)
  return 0;
}

int TagReader::next (void)
{ for (;;)
  { int tag = IO::ri32 (m_device); if (tag < 0) return -1;
    int size = IO::ri32 (m_device); if (size < 0) return -1;

    QTextStream (stdout) << "TagReader::next: tag=" << QString::number (tag, 16) << ", size=" << size << Qt::endl;

    if (tag == m_tag) return size;

    if (m_device->skip (size) != size) return -1;
  }
}
