#include "spatialdata.h"

const double SpatialData::ticksPerRotation = 16384.0;

SpatialData::SpatialData (void) : m_timestamp (0), m_channel (-1)
{
}

SpatialData::SpatialData (const TimeData &data) : m_timestamp (data.timestamp ()), m_channel (data.channel ())
{ QList<int> encoder = data.encoder ();
  QList<double> adc = data.adc ();

  // Get minimum and maximum encoder tick count.
  int encoderMin = encoder.at (0);
  int encoderMax = encoder.at (0);

  for (int sample = 1; sample < data.numOfSamples (); sample++)
  { encoderMin = qMin (encoderMin, encoder.at (sample));
    encoderMax = qMax (encoderMax, encoder.at (sample));
  }

  // Allocate memory for encoder count table.
  int encoderLength = (encoderMax - encoderMin) + 1;
  int *encoderCount = new int [encoderLength] ();

  // Count how many times each encoder tick count value appears in data.
  for (int sample = 0; sample < data.numOfSamples (); sample++) encoderCount [encoder.at (sample) - encoderMin]++;

  // Build angular position table.
  for (int index = 0; index < encoderLength; index++) if (encoderCount [index]) m_position.append (static_cast<double> (index + encoderMin) * (360.0 / ticksPerRotation));

  // Build spatial domain ADC table.
  double *temp = new double [encoderLength] ();

  // Sum ADC values that appear at the same encoder tick count.
  for (int sample = 0; sample < encoder.length (); sample++)
    temp [encoder.at (sample) - encoderMin] += adc.at (sample);

  m_adc.reserve (encoderLength);

  // Average ADC values per encoder tick count.
  for (int index = 0; index < encoderLength; index++)
    if (encoderCount [index]) m_adc.append (temp [index] / static_cast<double> (encoderCount [index]));

  delete [] temp;
  delete [] encoderCount;
}

int SpatialData::timestamp (void) const
{ return m_timestamp;
}

int SpatialData::numOfSamples (void) const
{ return m_adc.count ();
}

int SpatialData::channel (void) const
{ return m_channel;
}

QList<double> SpatialData::adc (void) const
{ return m_adc;
}

QList<double> SpatialData::position (void) const
{ return m_position;
}

QList<SpatialData> SpatialData::process (const QList<TimeData> &data)
{ QList<SpatialData> result;
  for (const TimeData &t : data) result.append (SpatialData (t));
  return result;
}
