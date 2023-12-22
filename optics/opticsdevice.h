#ifndef OPTICSDEVICE_H
#define OPTICSDEVICE_H

#include <QObject>
#include <QQueue>
#include "../../lib/fifo.h"
#include "../../lib/log.h"
#include "../../lib/usb/usbdevice.h"
#include "../../lib/usb/usbinterrupt.h"
#include "../../lib/usb/usbreader.h"
#include "captureconfiguration.h"
#include "ledconfiguration.h"
#include "rawdata.h"

class OpticsDevice : public QObject
{ Q_OBJECT

  public:
    explicit OpticsDevice (Log *log, QObject *parent = nullptr);
    ~OpticsDevice (void);

    bool isOpen (void) const;

    bool open (const UsbDeviceInfo &info);
    void close (void);

    bool ledOn (const int channelMask, const double duration = qSNaN ());
    bool ledOff (const int channelMask);
    bool ledConfigure (const int channel, const LedConfiguration &config);

    bool captureConfigure (const CaptureConfiguration &config);
    bool captureEnable (void);
    bool captureDisable (void);

    bool diskIndexEnable (void);
    bool diskIndexDisable (void);

    bool timerReset (void);

    bool encoderStart (const double interval);
    bool encoderStop (void);

    static QList<UsbDeviceInfo> listDevices (void);

  signals:
    void dataReceived (const RawData &data, const CaptureConfiguration &config);
    void encoderReceived (const QByteArray &data);

  private slots:
    void interruptReceived (const QByteArray &data);
    void readyReadAdc (void);
    void readyReadEncoder (void);
    void readyReadEncoderSampler (void);
    void errorOccurred (const int error);

  private:
    static const int vendorId = 0xb00b;
    static const int productId = 0x1235;

    static const int reqLedOn = 0x10;
    static const int reqLedOff = 0x11;
    static const int reqLedVSense = 0x12;
    static const int reqCaptureConfigNumOfItems = 0x20;
    static const int reqCaptureConfigTrigger = 0x21;
    static const int reqCaptureConfigPhotodiodeChannelMask = 0x22;
    static const int reqCaptureConfigLedChannelMask = 0x23;
    static const int reqCaptureConfigNumOfSamples = 0x24;
    static const int reqCaptureConfigEncoderCountStart = 0x25;
    static const int reqCaptureConfigEncoderCountEnd = 0x26;
    static const int reqCaptureEnable = 0x30;
    static const int reqCaptureDisable = 0x31;
    static const int reqDiskIndexEnable = 0x40;
    static const int reqDiskIndexDisable = 0x41;
    static const int reqTimerReset = 0x50;
    static const int reqEncoder = 0x60;

    static const int intDiskIndexFound = 0x01;
    static const int intOptoSync = 0x02;
    static const int intCaptureFinished = 0x03;

    Log *m_log;
    UsbDevice m_device;
    UsbInterrupt m_interrupt;
    UsbReader m_readerAdc;
    UsbReader m_readerEncoder;
    UsbReader m_readerEncoderSampler;

    CaptureConfiguration m_config;
    int m_diskIndex;
    int m_optoSync;

    QQueue<int> m_queueTimestamp;
    QQueue<int> m_queueAdcCount;
    QQueue<int> m_queueEncoderCount;

    Fifo<uint16_t> m_fifoAdc;
    Fifo<uint16_t> m_fifoEncoder;
    uint16_t m_bufferAdc [32];
    uint16_t m_bufferEncoder [32];

    void diskIndexFound (const int timestamp, const QByteArray &data);
    void optoSync (const int timestamp, const QByteArray &data);
    void captureFinished (const int timestamp, const QByteArray &data);
    void processFifo (void);

    static int u16 (const QByteArray &data, const int offset);
    static int i32 (const QByteArray &data, const int offset);
};

#endif // OPTICSDEVICE_H
