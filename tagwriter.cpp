#include "io.h"
#include "tagwriter.h"

TagWriter::TagWriter (QIODevice *device, const int tag, const int cacheSize) : m_device (device), m_tag (tag), m_cacheSize (cacheSize), m_cache (nullptr)
{
}

TagWriter::~TagWriter (void)
{ close ();
}

bool TagWriter::open (OpenMode mode)
{ if (m_cache != nullptr) return false;
  if (!QIODevice::open (mode)) return false;

  m_cache = new char [m_cacheSize];
  if (!m_cache) return false;

  m_index = 0;

  return true;
}

void TagWriter::close (void)
{ if (m_cache)
  { flush ();

    delete [] m_cache;
    m_cache = nullptr;
  }

  QIODevice::close ();
}

bool TagWriter::isSequential (void) const
{ return true;
}

bool TagWriter::flush (void)
{ if (!m_cache) return false;

  if (m_index)
  { if (!IO::wi32 (m_device, m_tag)) return false;
    if (!IO::wi32 (m_device, m_index)) return false;
    if (m_device->write (m_cache, m_index) != m_index) return false;

    m_index = 0;
  }

  return true;
}

qint64 TagWriter::readData (char *data, qint64 maxSize)
{ Q_UNUSED(data)
  Q_UNUSED(maxSize)
  return 0;
}

qint64 TagWriter::writeData (const char *data, qint64 maxSize)
{ if (!m_cache) return 0;

  int size = m_index + maxSize;
  if (size >= m_cacheSize)
  { if (!IO::wi32 (m_device, m_tag)) return 0;
    if (!IO::wi32 (m_device, size)) return 0;

    if (m_index)
    { if (m_device->write (m_cache, m_index) != m_index) return 0;
      m_index = 0;
    }

    if (m_device->write (data, maxSize) != maxSize) return 0;
  } else
  { memcpy (&m_cache [m_index], data, maxSize);
    m_index += maxSize;
  }

  return maxSize;
}
