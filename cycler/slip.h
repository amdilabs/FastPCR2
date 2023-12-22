#ifndef SLIP_H
#define SLIP_H

#include <QByteArray>
#include <QObject>
#include <QSerialPort>
#include "../../lib/log.h"

class Slip : public QObject
{ Q_OBJECT

  public:
    explicit Slip (Log *log, QObject *parent = nullptr);
    ~Slip (void);

    bool isOpen (void) const;

    bool open (const QString &portName);
    void close (void);

    bool write (const QByteArray &packet);

  signals:
    void packetReceived (const QByteArray &packet);

  private slots:
    void readyRead (void);

  private:
    static const char end = 0xc0;
    static const char esc = 0xdb;
    static const char escEnd = 0xdc;
    static const char escEsc = 0xdd;

    Log *m_log;
    QSerialPort *m_port;
    bool m_escape;
    QByteArray m_buffer;
};

#endif // SLIP_H
