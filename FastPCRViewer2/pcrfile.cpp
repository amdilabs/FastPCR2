#include "filereader.h"
#include "pcrfile.h"

PcrFile::PcrFile (QObject *parent) : QObject (parent)
{
}

bool PcrFile::load (const QString &fileName)
{ m_timeData.clear ();
  m_spatialData.clear ();

  m_captureConfig = FileReader::readCaptureConfig (fileName);
  m_cuvetteConfig = FileReader::readCuvetteLocationConfig (fileName);


//  // Create channels
//  QList<int> channels;
//  int numOfChannels = sizeOf(m_cuvetteConfig.m_config[]);
//  for (int index=0; index<numOfChannels; index++) channles.append(m_cuvetteConfig.m_config[index]);


  m_rawData = FileReader::readRawData (fileName);
  if (m_rawData.isEmpty ()) return false;

  for (int index = 0; index < m_rawData.count (); index++)
  { QList<TimeData> timeData = TimeData::process (m_rawData.at (index), m_captureConfig);
    m_timeData.append (timeData);
    m_spatialData.append (SpatialData::process (timeData));
  }

  m_temperatures = FileReader::readTemperatures (fileName);
  m_userInput = FileReader::readUserInput (fileName);

  return true;
}

int PcrFile::numOfCycles (void) const
{ return m_rawData.count ();
}

CaptureConfiguration PcrFile::captureConfig (void) const
{ return m_captureConfig;
}

CuvetteLocationConfigurationContainer PcrFile::cuvetteLocationConfig (void) const
{ return m_cuvetteConfig;
}

RawData PcrFile::rawData (const int cycle) const
{ return ((cycle >= 1) && (cycle <= m_rawData.count ())?m_rawData.at (cycle - 1):RawData ());
}

TemperatureData PcrFile::temperatures (void) const
{ return m_temperatures;
}

QList<TimeData> PcrFile::timeData (const int cycle) const
{ return ((cycle >= 1) && (cycle <= m_timeData.count ())?m_timeData.at (cycle -1):QList<TimeData> ());
}

QList<SpatialData> PcrFile::spatialData (const int cycle) const
{ return ((cycle >= 1) && (cycle <= m_spatialData.count ())?m_spatialData.at (cycle -1):QList<SpatialData> ());
}

QList<QPair<QString, QString> > PcrFile::userInput (void) const
{ return m_userInput;
}

void PcrFile::process (void)
{
}
