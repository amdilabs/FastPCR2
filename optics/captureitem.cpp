#include "captureitem.h"

const CaptureItem::Trigger CaptureItem::defaultTrigger = OptoSync;
const int CaptureItem::defaultPhotodiodeChannelMask = (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0);
const int CaptureItem::defaultLedChannelMask = (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0);
const int CaptureItem::defaultNumOfSamples = 15000;
const int CaptureItem::defaultEncoderCountStart = 0;
const int CaptureItem::defaultEncoderCountEnd = 0;

CaptureItem::CaptureItem (void) :
  m_trigger (defaultTrigger),
  m_photodiodeChannelMask (defaultPhotodiodeChannelMask),
  m_ledChannelMask (defaultLedChannelMask),
  m_numOfSamples (defaultNumOfSamples),
  m_encoderCountStart (defaultEncoderCountStart),
  m_encoderCountEnd (defaultEncoderCountEnd)
{
}

CaptureItem::CaptureItem (const Trigger trigger, const int photodiodeChannelMask, const int ledChannelMask, const int numOfSamples, const int encoderCountStart, const int encoderCountEnd) :
  m_trigger (trigger),
  m_photodiodeChannelMask (photodiodeChannelMask),
  m_ledChannelMask (ledChannelMask),
  m_numOfSamples (numOfSamples),
  m_encoderCountStart (encoderCountStart),
  m_encoderCountEnd (encoderCountEnd)
{
}

CaptureItem::CaptureItem (const QJsonObject &object) :
  m_trigger (triggerFromJson (object.value ("trigger"))),
  m_photodiodeChannelMask (object.value ("photodiodeChannelMask").toInt (defaultPhotodiodeChannelMask)),
  m_ledChannelMask (object.value ("ledChannelMask").toInt (defaultLedChannelMask)),
  m_numOfSamples (object.value ("numOfSamples").toInt (defaultNumOfSamples)),
  m_encoderCountStart (object.value ("encoderCountStart").toInt (defaultEncoderCountStart)),
  m_encoderCountEnd (object.value ("encoderCountEnd").toInt (defaultEncoderCountEnd))
{
}

CaptureItem::Trigger CaptureItem::trigger (void) const
{ return m_trigger;
}

int CaptureItem::photodiodeChannelMask (void) const
{ return m_photodiodeChannelMask;
}

int CaptureItem::ledChannelMask (void) const
{ return m_ledChannelMask;
}

int CaptureItem::numOfSamples (void) const
{ return m_numOfSamples;
}

int CaptureItem::encoderCountStart (void) const
{ return m_encoderCountStart;
}

int CaptureItem::encoderCountEnd (void) const
{ return m_encoderCountEnd;
}

void CaptureItem::setTrigger (const Trigger trigger)
{ m_trigger = trigger;
}

void CaptureItem::setPhotodiodeChannelMask (const int photodiodeChannelMask)
{ m_photodiodeChannelMask = photodiodeChannelMask;
}

void CaptureItem::setLedChannelMask (const int ledChannelMask)
{ m_ledChannelMask = ledChannelMask;
}

void CaptureItem::setNumOfSamples (const int numOfSamples)
{ m_numOfSamples = numOfSamples;
}

void CaptureItem::setEncoderCountStart (const int encoderCountStart)
{ m_encoderCountStart = encoderCountStart;
}

void CaptureItem::setEncoderCountEnd (const int encoderCountEnd)
{ m_encoderCountEnd = encoderCountEnd;
}

int CaptureItem::expectedDataSize (void) const
{ int numOfChannels = 1; // Start at 1 to include encoder data in count.
  for (int channel = 0; channel < 32; channel++) if (m_photodiodeChannelMask & (1 << channel)) numOfChannels++;
  return numOfChannels * m_numOfSamples * sizeof (uint16_t);
}

QJsonObject CaptureItem::toJson (void) const
{ QJsonObject object;
  object.insert ("trigger", triggerToJson (m_trigger));
  object.insert ("photodiodeChannelMask", m_photodiodeChannelMask);
  object.insert ("ledChannelMask", m_ledChannelMask);
  object.insert ("numOfSamples", m_numOfSamples);
  object.insert ("encoderCountStart", m_encoderCountStart);
  object.insert ("encoderCountEnd", m_encoderCountEnd);
  return object;
}

CaptureItem::Trigger CaptureItem::triggerFromJson (const QJsonValue &value)
{ QString str = value.toString ();
  if (str == "optoSync") return OptoSync;
  if (str == "encoderCount") return EncoderCount;
  if (str == "software") return Software;
  return defaultTrigger;
}

QJsonValue CaptureItem::triggerToJson (const Trigger trigger)
{ switch (trigger)
  { case OptoSync: return QJsonValue ("optoSync");
    case EncoderCount: return QJsonValue ("encoderCount");
    case Software: return QJsonValue ("software");
  }

  return triggerToJson (defaultTrigger);
}
