#include "cuvettelocationconfigurationcontainer.h"

CuvetteLocationConfigurationContainer::CuvetteLocationConfigurationContainer (void)
{
}

CuvetteLocationConfigurationContainer::CuvetteLocationConfigurationContainer (const QJsonArray &array)
{ for (const QJsonValue &value : array)
  { QJsonObject object = value.toObject ();
    m_config.insert (object.value ("channel").toInt (), CuvetteLocationConfiguration (object.value ("configuration").toObject ()));
  }
}

int CuvetteLocationConfigurationContainer::count (void) const
{ return m_config.count ();
}

QList<int> CuvetteLocationConfigurationContainer::channels (void) const
{ return m_config.keys ();
}

bool CuvetteLocationConfigurationContainer::haveConfiguration (const int channel) const
{ return m_config.contains (channel);
}

CuvetteLocationConfiguration CuvetteLocationConfigurationContainer::configuration (const int channel) const
{ return m_config.value (channel);
}

void CuvetteLocationConfigurationContainer::setConfiguration (const int channel, const CuvetteLocationConfiguration &config)
{ m_config [channel] = config;
}

void CuvetteLocationConfigurationContainer::init (const int channelMask)
{ for (int channel = 0; channel < 32; channel++)
    if ((channelMask & (1 << channel)) && !m_config.contains (channel))
      m_config.insert (channel, CuvetteLocationConfiguration ());
}

QJsonArray CuvetteLocationConfigurationContainer::toJson (void) const
{ QJsonArray array;

  QList<int> channels = m_config.keys ();
  for (const int channel : channels)
  { QJsonObject object;
    object.insert ("channel", channel);
    object.insert ("configuration", m_config.value (channel).toJson ());
    array.append (object);
  }

  return array;
}
