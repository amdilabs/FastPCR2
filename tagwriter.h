#ifndef TAGWRITER_H
#define TAGWRITER_H

#include <QIODevice>

class TagWriter : public QIODevice
{ public:
    TagWriter (QIODevice *device, const int tag, const int cacheSize = 65536);
    ~TagWriter (void);

    virtual bool open (OpenMode mode) override;
    virtual void close (void) override;

    virtual bool isSequential (void) const override;

    bool flush (void);

  protected:
    virtual qint64 readData (char *data, qint64 maxSize) override;
    virtual qint64 writeData (const char *data, qint64 maxSize) override;

  private:
    QIODevice *m_device;
    int m_tag;
    int m_index;
    int m_cacheSize;
    char *m_cache;
};

#endif // TAGWRITER_H
