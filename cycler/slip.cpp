#include "slip.h"

Slip::Slip (Log *log, QObject *parent) : QObject (parent), m_log (log)
{ m_port = nullptr;
}

Slip::~Slip (void)
{ close ();
}

bool Slip::isOpen (void) const
{ return m_port != nullptr;
}

bool Slip::open (const QString &portName)
{ close ();

  m_port = new QSerialPort (portName);
  m_port->setBaudRate (576000);
  m_port->setDataBits (QSerialPort::Data8);
  m_port->setStopBits (QSerialPort::OneStop);
  m_port->setParity (QSerialPort::NoParity);
  m_port->setFlowControl (QSerialPort::NoFlowControl);

  if (!m_port->open (QSerialPort::ReadWrite))
  { delete m_port;
    m_port = nullptr;
    m_log->error ("Slip", "Cannot open port '" + portName + "'.");
    return false;
  }

  m_escape = false;

  connect (m_port, &QSerialPort::readyRead, this, &Slip::readyRead);

  return true;
}

void Slip::close (void)
{ if (m_port)
  { m_port->close ();
    delete m_port;
    m_port = nullptr;

    m_buffer.clear ();
  }
}

bool Slip::write (const QByteArray &packet)
{ if (!m_port) return false;

  for (char ch : packet)
  { switch (ch)
    { case end:
        if (!m_port->putChar (esc)) return false;
        if (!m_port->putChar (escEnd)) return false;
        break;

      case esc:
        if (!m_port->putChar (esc)) return false;
        if (!m_port->putChar (escEsc)) return false;
        break;

      default:
        if (!m_port->putChar (ch)) return false;
    }
  }

  if (!m_port->putChar (end)) return false;

  return m_port->flush ();
}

void Slip::readyRead (void)
{ while (m_port->bytesAvailable ())
  { char ch;
    if (!m_port->getChar (&ch)) break;

    switch (ch)
    { case end:
        emit packetReceived (m_buffer);
        m_buffer.clear ();
        break;

      case esc:
        m_escape = true;
        break;

      case escEnd:
        if (m_escape)
        { m_escape = false;
          m_buffer.append (end);
        } else m_buffer.append (ch);
        break;

      case escEsc:
        if (m_escape)
        { m_escape = false;
          m_buffer.append (esc);
        } else m_buffer.append (ch);
        break;

      default:
        if (m_escape)
        { m_log->error ("Slip", "Invalid escape value.");
          m_escape = false;
          m_buffer.clear ();
        } else m_buffer.append (ch);
    }
  }
}
