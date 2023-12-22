#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>

class Message
{ public:
    Message (void);
    Message (const int type, const int id);
    Message (const QByteArray &data);

    bool isValid (void) const;
    int length (void) const;

    int type (void) const;
    int id (void) const;

    QByteArray getArray (const int offset, const int length = -1) const;
    bool getBool (const int offset) const;
    float getFloat (const int offset) const;
    int getInt8 (const int offset) const;
    int getInt32 (const int offset) const;
    QString getString (const int offset, const int lenght = -1) const;
    int getUint8 (const int offset) const;
    int getUint16 (const int offset) const;

    void appendArray (const QByteArray &array);
    void appendBool (const bool value);
    void appendFloat (const float value);
    void appendInt8 (const int value);
    void appendInt16 (const int value);
    void appendInt32 (const int value);

    QByteArray data (void) const;

  private:
    int m_type;
    int m_id;
    QByteArray m_data;
};

#endif // MESSAGE_H
