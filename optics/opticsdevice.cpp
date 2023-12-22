#include <QFile>
#include "opticsdevice.h"

OpticsDevice::OpticsDevice (Log *log, QObject *parent) :
  QObject (parent),
  m_log (log),
  m_interrupt (&m_device),
  m_readerAdc (&m_device, 2, 64, 65536),
  m_readerEncoder (&m_device, 3, 64, 32768),
  m_readerEncoderSampler (&m_device, 4, 64, 1024),
  m_fifoAdc (65536),
  m_fifoEncoder (32768)
{ connect (&m_interrupt, &UsbInterrupt::interruptReceived, this, &OpticsDevice::interruptReceived);
  connect (&m_interrupt, &UsbInterrupt::errorOccurred, this, &OpticsDevice::errorOccurred);
  connect (&m_readerAdc, &UsbReader::readyRead, this, &OpticsDevice::readyReadAdc);
  connect (&m_readerAdc, &UsbReader::errorOccurred, this, &OpticsDevice::errorOccurred);
  connect (&m_readerEncoder, &UsbReader::readyRead, this, &OpticsDevice::readyReadEncoder);
  connect (&m_readerEncoder, &UsbReader::errorOccurred, this, &OpticsDevice::errorOccurred);
  connect (&m_readerEncoderSampler, &UsbReader::readyRead, this, &OpticsDevice::readyReadEncoderSampler);
  connect (&m_readerEncoderSampler, &UsbReader::errorOccurred, this, &OpticsDevice::errorOccurred);
}

OpticsDevice::~OpticsDevice (void)
{ close ();
}

bool OpticsDevice::isOpen (void) const
{ return m_device.isOpen ();
}

bool OpticsDevice::open (const UsbDeviceInfo &info)
{ close ();

  if (!m_device.open (info))
  { m_log->error ("OpticsDevice", "Cannot open device.");
    return false;
  }
/*
  if (!m_device.reset ())
  { m_log->error ("OpticsDevice", "Cannot reset device.");
    m_device.close ();
    return false;
  }
*/

  if (!m_interrupt.open (1, 16))
  { m_log->error ("OpticsDevice", "Cannot open interrupt reader.");
    m_device.close ();
    return false;
  }

  //QThread::msleep (50);

  if (!m_readerAdc.open (UsbReader::ReadOnly))
  { m_log->error ("OpticsDevice", "Cannot open ADC reader.");
    m_interrupt.close ();
    m_device.close ();
    return false;
  }

  if (!m_readerEncoder.open (UsbReader::ReadOnly))
  { m_log->error ("OpticsDevice", "Cannot open encoder reader.");
    m_readerAdc.close ();
    m_interrupt.close ();
    m_device.close ();
    return false;
  }

  if (!m_readerEncoderSampler.open (UsbReader::ReadOnly))
  { m_log->error ("OpticsDevice", "Cannot open encoder sampler reader.");
    m_readerEncoder.close ();
    m_readerAdc.close ();
    m_interrupt.close ();
    m_device.close ();
    return false;
  }

  m_diskIndex = 0;
  m_optoSync = 0;

  return true;
}

void OpticsDevice::close (void)
{ if (m_device.isOpen ())
  { captureDisable ();

    m_readerEncoderSampler.close ();
    m_readerEncoder.close ();
    m_readerAdc.close ();
    m_interrupt.close ();
    m_device.close ();
  }
}

bool OpticsDevice::ledOn (const int channelMask, const double duration)
{ m_log->debug ("OpticsDevice", "LED on, channelMask=" + QString::number (channelMask) + ".");
  return m_device.controlTransfer (reqLedOn, channelMask, (qIsNaN (duration)?0:qBound (0, qRound (duration * 1000.0), 65535)));
}

bool OpticsDevice::ledOff (const int channelMask)
{ m_log->debug ("OpticsDevice", "LED off, channelMask=" + QString::number (channelMask) + ".");
  return m_device.controlTransfer (reqLedOff, channelMask, 0);
}

bool OpticsDevice::ledConfigure (const int channel, const LedConfiguration &config)
{ int vSense = qBound (0, qRound (config.vSense () * 1000.0), 100);
  m_log->debug ("OpticsDevice", "LED configure, channel=" + QString::number (channel) + ", current=" + QString::number (config.current (), 'f', 3) + " A, shunt=" + QString::number (config.shunt () * 1000.0, 'f', 1) + " mΩ, vSense=" + QString::number (vSense) + " mV.");
  return m_device.controlTransfer (reqLedVSense, vSense, channel);
}

bool OpticsDevice::captureConfigure (const CaptureConfiguration &config)
{ QList<CaptureItem> items = config.items ();

  if (!m_device.controlTransfer (reqCaptureConfigNumOfItems, items.count (), 0)) return false;

  for (int index = 0; index < items.count (); index++)
  { CaptureItem item = items.at (index);

    if (!m_device.controlTransfer (reqCaptureConfigTrigger, item.trigger (), index)) return false;
    if (!m_device.controlTransfer (reqCaptureConfigPhotodiodeChannelMask, item.photodiodeChannelMask (), index)) return false;
    if (!m_device.controlTransfer (reqCaptureConfigLedChannelMask, item.ledChannelMask (), index)) return false;
    if (!m_device.controlTransfer (reqCaptureConfigNumOfSamples, item.numOfSamples (), index)) return false;
    if (!m_device.controlTransfer (reqCaptureConfigEncoderCountStart, item.encoderCountStart (), index)) return false;
    if (!m_device.controlTransfer (reqCaptureConfigEncoderCountEnd, item.encoderCountEnd (), index)) return false;
  }

  m_config = config;
  return true;
}

bool OpticsDevice::captureEnable (void)
{ return m_device.controlTransfer (reqCaptureEnable, 0, 0);
}

bool OpticsDevice::captureDisable (void)
{ return m_device.controlTransfer (reqCaptureDisable, 0, 0);
}

bool OpticsDevice::diskIndexEnable (void)
{ return m_device.controlTransfer (reqDiskIndexEnable, 0, 0);
}

bool OpticsDevice::diskIndexDisable (void)
{ return m_device.controlTransfer (reqDiskIndexDisable, 0, 0);
}

bool OpticsDevice::timerReset (void)
{ return m_device.controlTransfer (reqTimerReset, 0, 0);
}

bool OpticsDevice::encoderStart (const double interval)
{ return m_device.controlTransfer (reqEncoder, qBound (0, qRound (interval * 1000.0), 174), 0);
}

bool OpticsDevice::encoderStop (void)
{ return m_device.controlTransfer (reqEncoder, 0, 0);
}

QList<UsbDeviceInfo> OpticsDevice::listDevices (void)
{ return UsbDeviceInfo::enumerate (vendorId, productId);
}

void OpticsDevice::interruptReceived (const QByteArray &data)
{ if (data.length () > 4)
  { int timestamp = i32 (data, 1);

    switch (data.at (0))
    { case intDiskIndexFound: diskIndexFound (timestamp, data); break;
      case intOptoSync: optoSync (timestamp, data); break;
      case intCaptureFinished: captureFinished (timestamp, data); break;
      default: m_log->warning ("OpticsDevice", "Unknown interrupt (" + QString::number (data.at (0)) + ").");
    }
  }
}

void OpticsDevice::readyReadAdc (void)
{ int remain = m_readerAdc.bytesAvailable ();

  while (remain)
  { int amount = qMin (remain, static_cast<int> (32 * sizeof (quint16)));
    m_readerAdc.read (reinterpret_cast<char *> (m_bufferAdc), amount);
    m_fifoAdc.put (amount / sizeof (uint16_t), m_bufferAdc);

    remain -= amount;
  }

  processFifo ();
}

void OpticsDevice::readyReadEncoder (void)
{ int remain = m_readerEncoder.bytesAvailable ();

  while (remain)
  { int amount = qMin (remain, static_cast<int> (32 * sizeof (qint16)));
    m_readerEncoder.read (reinterpret_cast<char *> (m_bufferEncoder), amount);
    m_fifoEncoder.put (amount / sizeof (uint16_t), m_bufferEncoder);

    remain -= amount;
  }

  processFifo ();
}

void OpticsDevice::readyReadEncoderSampler (void)
{ emit encoderReceived (m_readerEncoderSampler.readAll ());
}

void OpticsDevice::errorOccurred (const int error)
{ m_log->error ("OpticsDevice", "USB transfer error (" + QString::number (error) + ").");
}

void OpticsDevice::diskIndexFound (const int timestamp, const QByteArray &data)
{ Q_UNUSED(timestamp)

  if (data.length () == 9)
  { m_diskIndex = i32 (data, 5);

    m_log->info ("OpticsDevice", "Disk index found (" + QString::number (m_diskIndex) + ").");

  } else m_log->error ("OpticsDevice", "Truncated interrupt.");
}

void OpticsDevice::optoSync (const int timestamp, const QByteArray &data)
{ Q_UNUSED(timestamp)

  if (data.length () == 9)
  { m_optoSync = i32 (data, 5);

    m_log->info ("OpticsDevice", "Opto-sync (" + QString::number (m_optoSync) + ").");

  } else m_log->error ("OpticsDevice", "Truncated interrupt.");
}

void OpticsDevice::captureFinished (const int timestamp, const QByteArray &data)
{ if (data.length () == 9)
  { int adcCount = u16 (data, 5);
    int encoderCount = u16 (data, 7);

    m_queueTimestamp.enqueue (timestamp);
    m_queueAdcCount.enqueue (adcCount);
    m_queueEncoderCount.enqueue (encoderCount);

    m_log->info ("OpticsDevice", "Capture finished, adc=" + QString::number (adcCount) + ", encoder=" + QString::number (encoderCount) + ".");

  } else m_log->error ("OpticsDevice", "Truncated interrupt.");
}

void OpticsDevice::processFifo (void)
{ if (!m_queueAdcCount.isEmpty () && !m_queueEncoderCount.isEmpty ())
  { int adcCount = m_queueAdcCount.head ();
    int encoderCount = m_queueEncoderCount.head ();

    if ((m_fifoAdc.used () >= adcCount) && (m_fifoEncoder.used () >= encoderCount))
    { int timestamp = m_queueTimestamp.dequeue ();

      m_queueAdcCount.dequeue ();
      m_queueEncoderCount.dequeue ();

      uint16_t *adc = new uint16_t [adcCount];
      uint16_t *encoder = new uint16_t [encoderCount];

      m_fifoAdc.get (adcCount, adc);
      m_fifoEncoder.get (encoderCount, encoder);

      // No need to deallocate memory used by adc and encoder, RawData will take ownership allocated memory.
      emit dataReceived (RawData (timestamp, m_diskIndex, m_optoSync, adcCount, adc, encoderCount, encoder), m_config);
    }
   }
}

int OpticsDevice::u16 (const QByteArray &data, const int offset)
{ return static_cast<uint8_t> (data.at (offset)) + (static_cast<uint8_t> (data.at (offset + 1)) << 8);
}

int OpticsDevice::i32 (const QByteArray &data, const int offset)
{ return static_cast<uint8_t> (data.at (offset)) + (static_cast<uint8_t> (data.at (offset + 1)) << 8) + (static_cast<uint8_t> (data.at (offset + 2)) << 16) + (static_cast<uint8_t> (data.at (offset + 3)) << 24);
}
