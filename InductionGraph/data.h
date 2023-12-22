#ifndef DATA_H
#define DATA_H

#include <QDateTime>
#include <QString>

class Data
{ public:
    Data (void);
    Data (const QString &fileName);

    bool isValid (void) const;
    QString name (void) const;
    QDateTime timestamp (void) const;
    QList<double> time (void) const;
    QList<double> time (const double offset) const;
    QList<double> temperature (void) const;

  private:
    bool m_valid;
    QString m_name;
    QDateTime m_timestamp;
    QList<double> m_time;
    QList<double> m_temperature;
};

#endif // DATA_H
