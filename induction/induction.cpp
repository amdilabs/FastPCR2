#include <QElapsedTimer>
#include <QTextStream>
#include "induction.h"

Induction::Induction (QObject *parent) : QObject (parent), m_port (nullptr)
{
}

Induction::~Induction (void)
{ close ();
}

bool Induction::isOpen (void) const
{ return m_port != nullptr;
}

bool Induction::open (const QString &portName)
{ close ();

  m_port = new QSerialPort (portName);
  m_port->setBaudRate (QSerialPort::Baud115200);
  m_port->setDataBits (QSerialPort::Data8);
  m_port->setStopBits (QSerialPort::OneStop);
  m_port->setParity (QSerialPort::NoParity);
  m_port->setFlowControl (QSerialPort::NoFlowControl);

  if (!m_port->open (QSerialPort::ReadWrite))
  { delete m_port;
    m_port = nullptr;

    QTextStream (stderr) << "Induction: Cannot open port '" << portName << "'." << Qt::endl;
    return false;
  }

  m_seq = 0;
  return true;
}

void Induction::close (void)
{ if (m_port)
  { m_port->close ();
    delete m_port;
    m_port = nullptr;
  }
}

bool Induction::setPwmFreq (const int hz)
{ uint8_t in [] = {static_cast<uint8_t> (toBcd (hz / 10000)), static_cast<uint8_t> (toBcd (hz / 100)), static_cast<uint8_t> (toBcd (hz))};
  uint8_t out;

  return transfer (0x03, in, 3, &out, 1);
}

bool Induction::stopPwm (void)
{ uint8_t out;

  return transfer (0x04, nullptr, 0, &out, 1);
}

bool Induction::transfer (const int type, uint8_t *in, const int lengthIn, uint8_t *out, const int lengthOut)
{ if (!m_port) return false;

  uint8_t tx [16] = {0xfc, 0xfd, 0x0b, static_cast<uint8_t> (++m_seq), static_cast<uint8_t> (type), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfe};
  for (int index = 0; index < lengthIn; index++) tx [index + 5] = in [index];
  for (int index = 2; index < 13; index++) tx [13] += tx [index];

  QTextStream (stdout) << "tx: " << QString (QByteArray (reinterpret_cast<char *> (tx), 16).toHex ()) << Qt::endl;

  if (m_port->write (reinterpret_cast<char *> (tx), 16) != 16) return false;
  if (!m_port->flush ()) return false;

  QElapsedTimer timer;
  timer.start ();

  while ((timer.elapsed () < 1000) && (m_port->bytesAvailable () < 30)) m_port->waitForReadyRead (100);
  if (m_port->bytesAvailable () < 30) return false;

  uint8_t rx [30];
  if (m_port->read (reinterpret_cast <char *> (rx), 30) != 30) return false;

  QTextStream (stdout) << "rx: " << QString (QByteArray (reinterpret_cast<char *> (rx), 11).toHex ()) << Qt::endl;

  if (rx [0] != 0xfc) return false;
  if (rx [1] != 0xfd) return false;
  if (rx [2] != 0x19) return false;
  if (rx [3] != m_seq) return false;
  if (rx [4] != type) return false;

  uint8_t sum = 0;
  for (int index = 2; index < 27; index++) sum += rx [index];
  if (rx [27] != sum) return false;

  if (rx [28] != 0xfc) return false;
  if (rx [29] != 0xfe) return false;

  for (int index = 0; index < lengthOut; index++) out [index] = rx [index + 5];

  return true;
}

int Induction::toBcd (const int value)
{ return (((value / 10) % 10) << 4) + (value % 10);
}

int Induction::fromBcd (const int value)
{ return (((value >> 4) & 15) * 10) + (value & 15);
}
