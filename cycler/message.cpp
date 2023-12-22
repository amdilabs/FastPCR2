#include <QString>
#include <QTextStream>
#include "../../lib/crc/crc16.h"
#include "message.h"

Message::Message (void) : m_type (-1), m_id (-1)
{
}

Message::Message (const int type, const int id) : m_type (type), m_id (id)
{
}

Message::Message (const QByteArray &data) : m_type (-1), m_id (-1)
{ if (data.length () >= 4)
  { Crc16 crc16 = Crc16::modbus ();
    quint16 crcCalc = crc16.update (data.left (data.length () - 2));
    quint16 crcMsg = static_cast<quint8> (data.at (data.length () - 2)) + (static_cast<quint8> (data.at (data.length () - 1)) << 8);

    if (crcCalc == crcMsg)
    { m_type = static_cast<quint8> (data.at (0));
      m_id = static_cast<quint8> (data.at (1));
      m_data = data.mid (2, data.length () - 4);
    } else QTextStream (stdout) << "Message:: CRC error (" << QString::number (crcCalc, 16) << " != " << QString::number (crcMsg, 16) << ")." << Qt::endl;
  }
}

bool Message::isValid (void) const
{ return (m_type >= 0) && (m_id >= 0);
}

int Message::length (void) const
{ return m_data.length ();
}

int Message::type (void) const
{ return m_type;
}

int Message::id (void) const
{ return m_id;
}

QByteArray Message::getArray (const int offset, const int length) const
{ return m_data.mid (offset, length);
}

bool Message::getBool (const int offset) const
{ return m_data.at (offset) != 0;
}

float Message::getFloat (const int offset) const
{ qint32 temp = getInt32 (offset);
  return *reinterpret_cast<float *> (&temp);
}

int Message::getInt8 (const int offset) const
{ return m_data.at (offset);
}

int Message::getInt32 (const int offset) const
{ return static_cast<qint32> (static_cast<quint8> (m_data.at (offset)) + (static_cast<quint8> (m_data.at (offset + 1)) << 8) + (static_cast<quint8> (m_data.at (offset + 2)) << 16) + (static_cast<quint8> (m_data.at (offset + 3)) << 24));
}

QString Message::getString (const int offset, const int length) const
{ return QString::fromLatin1 (m_data.mid (offset, length));
}

int Message::getUint8 (const int offset) const
{ return static_cast<quint8> (m_data.at (offset));
}

int Message::getUint16 (const int offset) const
{ return static_cast<quint8> (m_data.at (offset)) + (static_cast<quint8> (m_data.at (offset + 1)) << 8);
}

void Message::appendArray (const QByteArray &array)
{ m_data.append (array);
}

void Message::appendBool (const bool value)
{ m_data.append (value?1:0);
}

void Message::appendFloat (const float value)
{ m_data.append (reinterpret_cast<const char *> (&value), 4);
}

void Message::appendInt8 (const int value)
{ m_data.append (value);
}

void Message::appendInt16 (const int value)
{ m_data.append (reinterpret_cast<const char *> (&value), 2);
}

void Message::appendInt32 (const int value)
{ m_data.append (reinterpret_cast<const char *> (&value), 4);
}

QByteArray Message::data (void) const
{ QByteArray result;
  result.append (m_type);
  result.append (m_id);
  result.append (m_data);

  Crc16 crc16 = Crc16::modbus ();
  quint16 crc = crc16.update (result);

  result.append (crc);
  result.append (crc >> 8);

  return result;
}
