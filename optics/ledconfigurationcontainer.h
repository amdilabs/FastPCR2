#ifndef LEDCONFIGURATIONCONTAINER_H
#define LEDCONFIGURATIONCONTAINER_H

#include <QJsonArray>
#include "ledconfiguration.h"

class LedConfigurationContainer
{ public:
    LedConfigurationContainer (void);
    LedConfigurationContainer (const QJsonArray &array);

    int count (void) const;
    QList<int> channels (void) const;

    LedConfiguration configuration (const int channel) const;
    void setConfiguration (const int channel, const LedConfiguration &config);

    QJsonArray toJson (void) const;

  private:
    QMap<int, LedConfiguration> m_config;
};

#endif // LEDCONFIGURATIONCONTAINER_H
