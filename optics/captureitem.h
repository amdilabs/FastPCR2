#ifndef CAPTUREITEM_H
#define CAPTUREITEM_H

#include <QJsonObject>

class CaptureItem
{ public:
    enum Trigger {OptoSync = 0, EncoderCount = 1, Software = 2};

    CaptureItem (void);
    CaptureItem (const Trigger trigger, const int photodiodeChannelMask, const int ledChannelMask, const int numOfSamples, const int encoderCountStart, const int encoderCountEnd);
    CaptureItem (const QJsonObject &object);

    Trigger trigger (void) const;
    int photodiodeChannelMask (void) const;
    int ledChannelMask (void) const;
    int numOfSamples (void) const;
    int encoderCountStart (void) const;
    int encoderCountEnd (void) const;

    void setTrigger (const Trigger trigger);
    void setPhotodiodeChannelMask (const int photodiodeChannelMask);
    void setLedChannelMask (const int ledChannelMask);
    void setNumOfSamples (const int numOfSamples);
    void setEncoderCountStart (const int encoderCountStart);
    void setEncoderCountEnd (const int encoderCountEnd);

    int expectedDataSize (void) const;

    QJsonObject toJson (void) const;

  private:
    static const Trigger defaultTrigger;
    static const int defaultPhotodiodeChannelMask;
    static const int defaultLedChannelMask;
    static const int defaultNumOfSamples;
    static const int defaultEncoderCountStart;
    static const int defaultEncoderCountEnd;

    Trigger m_trigger;
    int m_photodiodeChannelMask;
    int m_ledChannelMask;
    int m_numOfSamples;
    int m_encoderCountStart;
    int m_encoderCountEnd;

    static Trigger triggerFromJson (const QJsonValue &value);
    static QJsonValue triggerToJson (const Trigger trigger);
};

#endif // CAPTUREITEM_H
