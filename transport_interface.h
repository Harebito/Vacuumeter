#ifndef USBMONITOR_TRANSPORT_INTERFACE_H
#define USBMONITOR_TRANSPORT_INTERFACE_H

#include <QObject>

// It inherits QObject so we can use Signals/Slots
class TransportInterface : public QObject
{
    Q_OBJECT

public:
    explicit TransportInterface(QObject *parent = nullptr) : QObject(parent) {}

    // Virtual Destructor
    virtual ~TransportInterface() {}

    // Pure Virtual Functions
    virtual void connect_device(const QString &address) = 0;
    virtual void disconnect_device() = 0;
    virtual void send_data(const QByteArray &data) = 0;
    virtual bool is_connected() const = 0;

    signals:
        // Signals can be defined in the interface so you can connect to them generically
        void data_received(const QByteArray &data);
    void error_occurred(const QString &msg);
};

#endif // USBMONITOR_TRANSPORT_INTERFACE_H