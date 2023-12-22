#ifndef PGE_H
#define PGE_H

#include "../cycler/interface.h"

class Pge
{ public:
    enum Select {Heater65 = 0, Heater95 = 1, HeatSealer = 2};
    enum IoGroup {Group1 = 1, Group2 = 2, Group3 = 3, Group4 = 4};

    Pge (Interface *interface);

    double getIoGroupPosition (const Select select, const IoGroup group);
    double getIoGroupForce (const Select select, const IoGroup group);
    double getIoGroupSpeed (const Select select, const IoGroup group);

    bool setIoGroupPosition (const Select select, const IoGroup group, const double position);
    bool setIoGroupForce (const Select select, const IoGroup group, const double force);
    bool setIoGroupSpeed (const Select select, const IoGroup group, const double speed);

    bool saveParameter (const Select select);

  private:
    static const int slaveAddress = 1;
    static const int funcReadMultipleHoldingRegisters = 3;
    static const int funcWriteSingleHoldingRegister = 6;
    static const int funcWriteMultipleHoldingRegisters = 10;
    static const int funcReadDeviceIdentification = 43;

    Interface *m_interface;

    double readDouble (const Select select, const int registerAddress, const double multiplier);
};

#endif // PGE_H
