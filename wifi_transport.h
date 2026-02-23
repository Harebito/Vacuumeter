#ifndef USBMONITOR_WIFI_TRANSPORT_H
#define USBMONITOR_WIFI_TRANSPORT_H

#include <QObject>
#include <QTcpSocket>

// Assuming this inherits from some base interface, or just QObject
class WifiTransport : public QObject // or public ITransport
{
    Q_OBJECT

public:
    explicit WifiTransport(QObject *parent = nullptr);
    ~WifiTransport() override;

    // The methods causing linker errors:
    void connect_device(const QString &address);
    void disconnect_device();
    void send_data(const QByteArray &data);
    bool is_connected() const;

private slots:
    void on_socket_ready_read(); // The missing slot from your error log

private:
    QTcpSocket *m_socket;
};

#endif // USBMONITOR_WIFI_TRANSPORT_H