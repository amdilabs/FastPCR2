#include <QTextStream>
#include "pge.h"

Pge::Pge (Interface *interface) : m_interface (interface)
{
}

double Pge::getIoGroupPosition (const Select select, const IoGroup group)
{ return readDouble (select, 0x402 + 3 * group, 1.0e-3);
}

double Pge::getIoGroupForce (const Select select, const IoGroup group)
{ return readDouble (select, 0x403 + 3 * group, 1.0e-2);
}

double Pge::getIoGroupSpeed (const Select select, const IoGroup group)
{ return readDouble (select, 0x404 + 3 * group, 1.0e-2);
}

bool Pge::setIoGroupPosition (const Select select, const IoGroup group, const double position)
{ m_interface->sendGripper485 (select, slaveAddress, funcWriteSingleHoldingRegister, 0x402 + 3 * group, qBound (0, qRound (position * 1000.0), 1000));
  return true;
}

bool Pge::setIoGroupForce (const Select select, const IoGroup group, const double force)
{ m_interface->sendGripper485 (select, slaveAddress, funcWriteSingleHoldingRegister, 0x403 + 3 * group, qBound (20, qRound (force * 100.0), 100));
  return true;
}

bool Pge::setIoGroupSpeed (const Select select, const IoGroup group, const double speed)
{ m_interface->sendGripper485 (select, slaveAddress, funcWriteSingleHoldingRegister, 0x404 + 3 * group, qBound (1, qRound (speed * 100.0), 100));
  return true;
}

bool Pge::saveParameter (const Select select)
{ m_interface->sendGripper485 (select, slaveAddress, funcWriteSingleHoldingRegister, 0x300, 1);
  return true;
}

double Pge::readDouble (const Select select, const int registerAddress, const double multiplier)
{ int value = m_interface->sendGripper485 (select, slaveAddress, funcReadMultipleHoldingRegisters, registerAddress, 1);
  if (value < 0) return qSNaN ();

  return static_cast<double> (value) * multiplier;
}
