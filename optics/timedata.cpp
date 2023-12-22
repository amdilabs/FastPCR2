#include "timedata.h"

const double TimeData::adcGain = 3.3 / 4096.0;

TimeData::TimeData (void) : m_timestamp (0), m_channel (-1)
{
}

int TimeData::timestamp (void) const
{ return m_timestamp;
}

int TimeData::numOfSamples (void) const
{ return m_adc.count ();
}

int TimeData::channel (void) const
{ return m_channel;
}

QList<TimeData> TimeData::process (const RawData &data, const CaptureConfiguration &config)
{ QList<TimeData> result;

  uint16_t *adc = data.adc ();
  uint16_t *encoder = data.encoder ();

  for (const CaptureItem &item : config.items ())
  { QList<int> channels = TimeData::channels (item.photodiodeChannelMask ());

    // Create one instance for each photodiode channel.
    for (const int channel : channels)
      result.append (TimeData (item, data.timestamp (), data.diskIndex (), data.optoSync (), channel, channels.count (), adc, encoder));

    adc += item.numOfSamples () * channels.count ();
    encoder += item.numOfSamples ();
  }

  return result;
}

TimeData::TimeData (const CaptureItem &item, const int timestamp, const int diskIndex, const int optoSync, const int offset, const int stride, uint16_t *adc, uint16_t *encoder) : m_timestamp (timestamp), m_channel (offset)
{ if (item.numOfSamples () > 0)
  { m_adc.reserve (item.numOfSamples ());
    m_encoder.reserve (item.numOfSamples ());

    adc += offset;

    int encoderPrev = *encoder;
    bool first = true;

    // Multi-rotation correction.
    int multiRotation = (diskIndex & 0x3fff);
    while (multiRotation < encoderPrev) multiRotation += 16384;
    multiRotation -= 16384;

    /*
    if (encoderPrev >= 49152) multiRotation += 49152; else
    if (encoderPrev >= 32768) multiRotation += 32768; else
    if (encoderPrev >= 16384) multiRotation += 16384;
    */

    for (int index = 0; index < item.numOfSamples (); index++)
    { m_adc.append (static_cast<double> (*adc) * adcGain);
      adc += stride;

      int encoderNew = *encoder++;
      int encoderDiff = encoderNew - encoderPrev;

      // Wrap around correction.
      if (encoderDiff > 32767) encoderNew -= 65536; else
      if (encoderDiff < -32767) encoderNew += 65536;

      int encoderFinal = encoderNew - multiRotation;

      if (first)
      { first = false;
        QTextStream (stdout) << "encP=" << encoderPrev << ", encN=" << encoderNew << ", encF=" << encoderFinal << ", mr=" << multiRotation << ", index=" << diskIndex << ", opto=" << optoSync << Qt::endl;
      }

      m_encoder.append (encoderFinal);

      encoderPrev = encoderNew;
    }
  }
}

QList<int> TimeData::channels (const int channelMask)
{ QList<int> result;

  for (int channel = 0; channel < 32; channel++)
    if (channelMask & (1 << channel)) result.append (channel);

  return result;
}

QList<double> TimeData::adc (void) const
{ return m_adc;
}

QList<int> TimeData::encoder (void) const
{ return m_encoder;
}
