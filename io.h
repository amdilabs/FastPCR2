#ifndef IO_H
#define IO_H

#include <QIODevice>

class IO
{ public:
    static int ri16 (QIODevice *dev);
    static int ri32 (QIODevice *dev);

    static bool wi16 (QIODevice *dev, const int value);
    static bool wi32 (QIODevice *dev, const int value);
};

#endif // IO_H
