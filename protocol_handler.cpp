#include "protocol_handler.h"
#include "transport_interface.h"

ProtocolHandler::ProtocolHandler(QObject *parent)
    : QObject(parent), active_transport_(nullptr) {
}

void ProtocolHandler::set_transport(TransportInterface *transport) {
    active_transport_ = transport;
    
    // Listen to the transport, whatever it is
    connect(active_transport_, &TransportInterface::data_received, 
            this, &ProtocolHandler::process_incoming_raw_data);
}

// SCENARIO A: The "Pull" Approach (App requests F-RAM)
void ProtocolHandler::request_fram_dump() {
    if (active_transport_) {
        // Example: 0x01 is your custom "READ_FRAM" command byte
        QByteArray cmd;
        cmd.append((char)0x01); 
        active_transport_->send_data(cmd);
    }
}

// SCENARIO B: The "Push" Approach (Processing the stream)
void ProtocolHandler::process_incoming_raw_data(const QByteArray &raw_data) {
    // 1. Append to internal buffer (handles fragmentation)
    buffer_.append(raw_data);

    // 2. Simple Parse Logic (Example: Data ends with '\n')
    // If your ATmega sends "TEMP:24.5\n"
    int end_index = buffer_.indexOf('\n');
    while (end_index != -1) {
        QByteArray packet = buffer_.left(end_index);
        buffer_.remove(0, end_index + 1); // Remove processed data
        
        // Parse "TEMP:24.5"
        QString str_packet = QString::fromUtf8(packet).trimmed();
        emit log_message("Parsed: " + str_packet);
        
        end_index = buffer_.indexOf('\n');
    }
}