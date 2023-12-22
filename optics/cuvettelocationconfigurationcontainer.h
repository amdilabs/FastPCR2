#ifndef CUVETTELOCATIONCONFIGURATIONCONTAINER_H
#define CUVETTELOCATIONCONFIGURATIONCONTAINER_H

#include <QJsonArray>
#include "cuvettelocationconfiguration.h"

class CuvetteLocationConfigurationContainer
{ public:
    CuvetteLocationConfigurationContainer (void);
    CuvetteLocationConfigurationContainer (const QJsonArray &array);

    int count (void) const;
    QList<int> channels (void) const;

    bool haveConfiguration (const int channel) const;
    CuvetteLocationConfiguration configuration (const int channel) const;
    void setConfiguration (const int channel, const CuvetteLocationConfiguration &config);

    void init (const int channelMask);

    QJsonArray toJson (void) const;

  private:
    QMap<int, CuvetteLocationConfiguration> m_config;
};

#endif // CUVETTELOCATIONCONFIGURATIONCONTAINER_H
