#ifndef INDUCTION_H
#define INDUCTION_H

#include <QObject>
#include <QSerialPort>

class Induction : public QObject
{ Q_OBJECT

  public:
    explicit Induction (QObject *parent = nullptr);
    ~Induction (void);

    bool isOpen (void) const;

    bool open (const QString &portName);
    void close (void);

    bool setPwmFreq (const int hz);
    bool stopPwm (void);

  private:
    QSerialPort *m_port;
    int m_seq;

    bool transfer (const int type, uint8_t *in, const int lengthIn, uint8_t *out, const int lengthOut);

    static int toBcd (const int value);
    static int fromBcd (const int value);
};

#endif // INDUCTION_H
