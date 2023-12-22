#ifndef TAGREADER_H
#define TAGREADER_H

#include <QIODevice>

class TagReader : public QIODevice
{ public:
    TagReader (QIODevice *device, const int tag);
    ~TagReader (void);

    virtual bool open (OpenMode mode) override;
    virtual void close (void) override;

    virtual bool isSequential (void) const override;

  protected:
    virtual qint64 readData (char *data, qint64 maxSize) override;
    virtual qint64 writeData (const char *data, qint64 maxSize) override;

  private:
    QIODevice *m_device;
    int m_tag;
    int m_remain;

    int next (void);
};

#endif // TAGREADER_H
