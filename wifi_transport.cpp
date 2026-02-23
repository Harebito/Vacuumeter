#include "wifi_transport.h"
#include <QDebug> // If you need logging

WifiTransport::WifiTransport(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
{
    // Connect your signals/slots here
    connect(m_socket, &QTcpSocket::readyRead, this, &WifiTransport::on_socket_ready_read);
}

WifiTransport::~WifiTransport() {
    if (m_socket->isOpen()) {
        m_socket->close();
    }
}

void WifiTransport::connect_device(const QString &address) {
    // Basic implementation example
    m_socket->connectToHost(address, 1234); // Replace port as needed
}

void WifiTransport::disconnect_device() {
    m_socket->disconnectFromHost();
}

void WifiTransport::send_data(const QByteArray &data) {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(data);
    }
}

bool WifiTransport::is_connected() const {
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void WifiTransport::on_socket_ready_read() {
    QByteArray data = m_socket->readAll();
    // emit dataReceived(data); // If you have a signal for this
}