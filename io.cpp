#include "io.h"

int IO::ri16 (QIODevice *dev)
{ qint16 result;
  return (dev->read (reinterpret_cast<char *> (&result), 2) == 2?result:-1);
}

int IO::ri32 (QIODevice *dev)
{ qint32 result;
  return (dev->read (reinterpret_cast<char *> (&result), 4) == 4?result:-1);
}

bool IO::wi16 (QIODevice *dev, const int value)
{ char data [] = {static_cast<char> (value), static_cast<char> (value >> 8)};
  return (dev->write (data, 2) == 2);
}

bool IO::wi32 (QIODevice *dev, const int value)
{ char data [] = {static_cast<char> (value), static_cast<char> (value >> 8), static_cast<char> (value >> 16), static_cast<char> (value >> 24)};
  return (dev->write (data, 4) == 4);
}
