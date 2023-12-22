#ifndef DISKCONFIGURATION_H
#define DISKCONFIGURATION_H

#include <QJsonObject>
#include "valve.h"

class DiskConfiguration
{ public:
    DiskConfiguration (void);
    DiskConfiguration (const double cuvetteOffset, const double cuvetteSeparation, const double heater65Location, const double heater95Location, const Valve &hyperbaricValve, const Valve &meteringValve, const Valve &insertValve);
    DiskConfiguration (const QJsonObject &object);

    double cuvetteOffset (void) const;
    double cuvetteSeparation (void) const;
    double heater65Location (void) const;
    double heater95Location (void) const;
    Valve hyperbaricValve (void) const;
    Valve meteringValve (void) const;
    Valve insertValve (void) const;

    void setCuvetteOffset (const double cuvetteOffset);
    void setCuvetteSeparation (const double cuvetteSeparation);
    void setHeater65Location (const double heater65Location);
    void setHeater95Location (const double heater95Location);
    void setHyperbaricValve (const Valve &hyperbaricValve);
    void setMeteringValve (const Valve &meteringValve);
    void setInsertValve (const Valve &insertValve);

    int cuvetteOffsetTicks (const double ticksPerRevolution = 16384.0) const;
    int cuvetteSeparationTicks (const double ticksPerRevolution = 16384.0) const;
    int heater65LocationTicks (const double ticksPerRevolution = 16384.0) const;
    int heater95LocationTicks (const double ticksPerRevolution = 16384.0) const;

    double cuvetteLocation (const int cuvette) const;
    int cuvetteLocationTicks (const int cuvette, const double ticksPerRevolution = 16384.0) const;

    QJsonObject toJson (void) const;

  private:
    static const double defaultCuvetteOffset;
    static const double defaultCuvetteSeparation;
    static const double defaultHeater65Location;
    static const double defaultHeater95Location;
    static const Valve defaultHyperbaricValve;
    static const Valve defaultMeteringValve;
    static const Valve defaultInsertValve;

    double m_cuvetteOffset;
    double m_cuvetteSeparation;
    double m_heater65Location;
    double m_heater95Location;
    Valve m_hyperbaricValve;
    Valve m_meteringValve;
    Valve m_insertValve;

    static int ticks (const double degrees, const double ticksPerRevolution);
};

#endif // DISKCONFIGURATION_H
