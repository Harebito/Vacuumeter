#include "usb_transport.h"
#include <QDebug>

DeviceManager::DeviceManager(QObject *parent) : QObject(parent) {
    serial_port_ = new QSerialPort(this);

    // Connect hardware signal to our slot
    connect(serial_port_, &QSerialPort::readyRead, this, &DeviceManager::handle_ready_read);

    // Optional error logging
    connect(serial_port_, &QSerialPort::errorOccurred, this, [](QSerialPort::SerialPortError error){
        if (error != QSerialPort::NoError) {
            qDebug() << "Serial Error:" << error;
        }
    });
}

QStringList DeviceManager::scan_ports() {
    QStringList result;
    const auto ports = QSerialPortInfo::availablePorts();

    for (const auto &port : ports) {
        // FILTER: Skip ports that don't have a specific Vendor ID (hides ttyS*)
        if (!port.hasVendorIdentifier()) {
            continue;
        }

        QString entry = port.portName();
        if (!port.description().isEmpty()) {
            entry += " - " + port.description();
        }
        result.append(entry);
    }
    return result;
}

bool DeviceManager::connect_to_port(const QString &ui_text) {
    // 1. Reset if already open
    if (serial_port_->isOpen()) {
        serial_port_->close();
    }

    // 2. Parse name (Input: "ttyACM0 - Espressif" -> Use: "ttyACM0")
    QString port_name = ui_text.split(" ").first();

    serial_port_->setPortName(port_name);

    // 3. Standard Configuration
    serial_port_->setBaudRate(QSerialPort::Baud115200); // Adjust if your device uses a different rate
    serial_port_->setDataBits(QSerialPort::Data8);
    serial_port_->setParity(QSerialPort::NoParity);
    serial_port_->setStopBits(QSerialPort::OneStop);
    serial_port_->setFlowControl(QSerialPort::NoFlowControl);

    if (serial_port_->open(QIODevice::ReadWrite)) {
        qDebug() << "SUCCESS: Connected to" << port_name;
        emit connection_status_changed(true);
        return true;
    } else {
        qDebug() << "FAILED:" << serial_port_->errorString();
        emit connection_status_changed(false);
        return false;
    }
}

void DeviceManager::disconnect_device() {
    if (serial_port_->isOpen()) {
        serial_port_->close();
        emit connection_status_changed(false);
    }
}

void DeviceManager::handle_ready_read() {
    const QByteArray data = serial_port_->readAll();
    if (!data.isEmpty()) {
        // Pass raw data up to UI
        emit data_received(data);
    }
}