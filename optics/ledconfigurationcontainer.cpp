#include "ledconfigurationcontainer.h"

LedConfigurationContainer::LedConfigurationContainer (void)
{
}

LedConfigurationContainer::LedConfigurationContainer (const QJsonArray &array)
{ for (const QJsonValue &value : array)
  { QJsonObject object = value.toObject ();
    m_config.insert (object.value ("channel").toInt (), LedConfiguration (object.value ("configuration").toObject ()));
  }
}

int LedConfigurationContainer::count (void) const
{ return m_config.count ();
}

QList<int> LedConfigurationContainer::channels (void) const
{ return m_config.keys ();
}

LedConfiguration LedConfigurationContainer::configuration (const int channel) const
{ return m_config.value (channel);
}

void LedConfigurationContainer::setConfiguration (const int channel, const LedConfiguration &config)
{ m_config [channel] = config;
}

QJsonArray LedConfigurationContainer::toJson (void) const
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
