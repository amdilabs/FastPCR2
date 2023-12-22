#ifndef TAGS_H
#define TAGS_H

class Tags
{  public:
    static const int captureConfiguration = 0x63706163;          // capc
    static const int cuvetteLocationConfiguration = 0x6c767563;  // cuvl
    static const int encoder = 0x6f636e65;                       // enco
    static const int gripperConfiguration = 0x70697267;          // grip
    static const int log = 0x5f676f6c;                           // log_
    static const int pcrProtocol = 0x70726370;                   // pcrp
    static const int rawData = 0x61746164;                       // data
    static const int temperatures = 0x706d6574;                  // temp
    static const int userInput = 0x706e6975;                     // uinp
};

#endif // TAGS_H
