#ifndef USBMONITOR_PROTOCOL_HANDLER_H
#define USBMONITOR_PROTOCOL_HANDLER_H
#include <QObject>
#include "transport_interface.h"

class ProtocolHandler : public QObject {
    Q_OBJECT
public:
    explicit ProtocolHandler(QObject *parent = nullptr);

    // The UI gives this handler the active transport (USB or WiFi)
    void set_transport(TransportInterface *transport);

    // COMMANDS (The "Pull" Logic)
    // If the logic changes to "Request data first", you call this.
    void request_fram_dump();
    void send_heartbeat();

    signals:
        // High-level signals (Parsed data, not raw bytes)
        void sensor_update(float temp, float humidity);
    void log_message(const QString &msg);

private slots:
    // Handles raw bytes from USB or WiFi indiscriminately
    void process_incoming_raw_data(const QByteArray &raw_data);

private:
    TransportInterface *active_transport_;
    QByteArray buffer_; // To store fragmented packets
};
#endif // USBMONITOR_PROTOCOL_HANDLER_H