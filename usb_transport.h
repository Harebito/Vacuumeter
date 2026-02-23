#ifndef USBMONITOR_USB_TRANSPORT_H
#define USBMONITOR_USB_TRANSPORT_H
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class DeviceManager : public QObject {
    Q_OBJECT
public:
    explicit DeviceManager(QObject *parent = nullptr);

    // Returns formatted strings for the UI
    QStringList scan_ports();

    // Connects to the specific port
    bool connect_to_port(const QString &port_name);

    // Closes connection
    void disconnect_device();

    signals:
        void data_received(const QByteArray &data);
    void connection_status_changed(bool connected);

private slots:
    void handle_ready_read();

private:
    QSerialPort *serial_port_; // snake_case member variable
};

#endif // USBMONITOR_USB_TRANSPORT_H