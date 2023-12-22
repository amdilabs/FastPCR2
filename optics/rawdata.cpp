#include <QTextStream>
#include "../io.h"
#include "rawdata.h"

RawData::RawData (void) :
  m_valid (false),
  m_timestamp (0),
  m_diskIndex (0),
  m_optoSync (0),
  m_adcBytes (0),
  m_encoderBytes (0),
  m_data (new Data)
{
}

RawData::RawData (const int timestamp, const int diskIndex, const int optoSync, const int adcCount, uint16_t *adc, const int encoderCount, uint16_t *encoder) :
  m_valid (true),
  m_timestamp (timestamp),
  m_diskIndex (diskIndex),
  m_optoSync (optoSync),
  m_adcBytes (adcCount * sizeof (uint16_t)),
  m_encoderBytes (encoderCount * sizeof (uint16_t)),
  m_data (new Data (adc, encoder))
{
  //Ming: 7/19, uncommented
  QTextStream (stdout) << "#### RawData: di=" << diskIndex << ", os=" << optoSync << ", enc[0]=" << encoder [0] << Qt::endl;
}

RawData::RawData (QIODevice *device) :
  m_valid (false),
  m_timestamp (IO::ri32 (device)),
  m_diskIndex (IO::ri32 (device)),
  m_optoSync (IO::ri32 (device)),
  m_adcBytes (IO::ri32 (device)),
  m_encoderBytes (IO::ri32 (device))
{
  //Ming: 7/19, uncommented
  QTextStream (stdout) << "RawData::RawData: timestamp=" << m_timestamp << ", diskIndex=" << m_diskIndex << ", optoSync=" << m_optoSync << ", adcBytes=" << m_adcBytes << ", encoderBytes=" << m_encoderBytes << Qt::endl;

  if (m_adcBytes > 0)
  { uint16_t *adc = new uint16_t [(m_adcBytes + 1) / sizeof (uint16_t)];

    if (device->read (reinterpret_cast<char *> (adc), m_adcBytes) == m_adcBytes)
    { if (m_encoderBytes > 0)
      { uint16_t *encoder = new uint16_t [(m_encoderBytes + 1) / sizeof (uint16_t)];

        if (device->read (reinterpret_cast<char *> (encoder), m_encoderBytes) == m_encoderBytes)
        { m_data = new Data (adc, encoder);
          m_valid = true;

          return;
        }
      }
    }
  }

  m_data = new Data;
}

bool RawData::isValid (void) const
{ return m_valid;
}

int RawData::timestamp (void) const
{ return m_timestamp;
}

int RawData::diskIndex (void) const
{ return m_diskIndex;
}

int RawData::optoSync (void) const
{ return m_optoSync;
}

uint16_t *RawData::adc (void) const
{ return m_data->adc ();
}

uint16_t *RawData::encoder (void) const
{ return m_data->encoder ();
}

int RawData::size (void) const
{ return 20 + m_adcBytes + m_encoderBytes;
}

bool RawData::write (QIODevice *device) const
{ if (!IO::wi32 (device, m_timestamp)) return false;
  if (!IO::wi32 (device, m_diskIndex)) return false;
  if (!IO::wi32 (device, m_optoSync)) return false;
  if (!IO::wi32 (device, m_adcBytes)) return false;
  if (!IO::wi32 (device, m_encoderBytes)) return false;
  if (device->write (reinterpret_cast<char *> (m_data->adc ()), m_adcBytes) != m_adcBytes) return false;
  if (device->write (reinterpret_cast<char *> (m_data->encoder ()), m_encoderBytes) != m_encoderBytes) return false;
  return true;
}

RawData::Data::Data (void) : m_adc (nullptr), m_encoder (nullptr)
{
}

RawData::Data::Data (uint16_t *adc, uint16_t *encoder) : m_adc (adc), m_encoder (encoder)
{
}

RawData::Data::~Data (void)
{ if (m_adc) delete [] m_adc;
  if (m_encoder) delete [] m_encoder;
}

uint16_t *RawData::Data::adc (void) const
{ return m_adc;
}

uint16_t *RawData::Data::encoder (void) const
{ return m_encoder;
}
