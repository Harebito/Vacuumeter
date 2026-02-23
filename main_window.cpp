#include "main_window.h"

#include <qcoreapplication.h>
#include <QDebug>
#include <QDateTime>
#include "usb_transport.h"
#include "database_manager.h"

void MainWindow::setup_menu() {
    // 1. Create (or get) the Menu Bar
    // QMainWindow usually has one by default, but this ensures we use it.
    QMenuBar *menu_bar = this->menuBar();

    // 2. Create the "Help" Menu
    QMenu *help_menu = menu_bar->addMenu(tr("&Help"));

    // 3. Create the "About Qt" Action
    QAction *about_qt_action = new QAction(tr("About &Qt"), this);

    // Optional: Add a status tip (shows at bottom left of window on hover)
    about_qt_action->setStatusTip(tr("Show the Qt library's About box"));

    // 4. Connect the Action to the global Qt "About" slot
    // qApp is a global pointer to your QApplication instance
    connect(about_qt_action, &QAction::triggered, qApp, &QApplication::aboutQt);

    // 5. Add the action to the menu
    help_menu->addAction(about_qt_action);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    setup_menu();

    // Initialize backend
    device_manager_ = new DeviceManager(this);

    // Build the GUI
    setup_ui();

    // Initial scan on startup
    handle_scan();

    // CONNECT SIGNALS --------------------------------------------------

    // 1. Scan Button
    connect(scan_btn_, &QPushButton::clicked, this, &MainWindow::handle_scan);

    // 2. Connect Button
    connect(connect_btn_, &QPushButton::clicked, this, &MainWindow::handle_connect);

    // 3. Backend Signals (Data & Status)
    connect(device_manager_, &DeviceManager::data_received,
            this, &MainWindow::append_data_to_console);

    connect(device_manager_, &DeviceManager::connection_status_changed,
            this, &MainWindow::update_connection_state);

    db_manager_ = new DatabaseManager(this);
    if (db_manager_->init()) {
        qDebug() << "DB Initialized successfully";
    }

    // OPTION A: Save RAW data directly from Transport
    // connect(m_active_transport, &TransportInterface::data_received,
    //         m_db_manager, &DatabaseManager::save_data);

    // OPTION B (Better): Save processed data from ProtocolHandler
    // You might need to adjust save_data() to take struct/strings instead of QByteArray
    // connect(m_protocol_handler, &ProtocolHandler::packetParsed,
    //         m_db_manager, &DatabaseManager::save_data);
}

MainWindow::~MainWindow() {
    // Qt handles child cleanup automatically, but good practice to verify
}

void MainWindow::setup_ui() {
    setWindowTitle("USB Device Controller");
    resize(600, 400);

    central_widget_ = new QWidget(this);
    setCentralWidget(central_widget_);

    main_layout_ = new QVBoxLayout(central_widget_);

    // --- Top Control Bar (HBoxLayout) ---
    auto *controls_layout = new QHBoxLayout();

    port_selector_ = new QComboBox(this);
    port_selector_->setMinimumWidth(200);

    scan_btn_ = new QPushButton("Rescan", this);
    connect_btn_ = new QPushButton("Connect", this);

    controls_layout->addWidget(new QLabel("Port:"));
    controls_layout->addWidget(port_selector_);
    controls_layout->addWidget(scan_btn_);
    controls_layout->addWidget(connect_btn_);
    controls_layout->addStretch(); // Pushes buttons to the left

    // --- Data Display Area ---
    console_output_ = new QTextEdit(this);
    console_output_->setReadOnly(true);
    console_output_->setPlaceholderText("Waiting for device data...");

    // --- Status Bar ---
    status_label_ = new QLabel("Ready. Select a port.", this);

    // Add everything to main layout
    main_layout_->addLayout(controls_layout);
    main_layout_->addWidget(console_output_);
    main_layout_->addWidget(status_label_);
}

void MainWindow::handle_scan() {
    port_selector_->clear();

    QStringList ports = device_manager_->scan_ports();

    if (ports.isEmpty()) {
        port_selector_->addItem("No Devices Found");
        port_selector_->setDisabled(true);
        connect_btn_->setDisabled(true);
    } else {
        port_selector_->addItems(ports);
        port_selector_->setDisabled(false);
        connect_btn_->setDisabled(false);
    }
}

void MainWindow::handle_connect() {
    QString current_text = port_selector_->currentText();

    // Check if we are trying to Connect or Disconnect
    if (connect_btn_->text() == "Connect") {
        device_manager_->connect_to_port(current_text);
    } else {
        device_manager_->disconnect_device();
    }
}

void MainWindow::update_connection_state(bool is_connected) {
    if (is_connected) {
        connect_btn_->setText("Disconnect");
        status_label_->setText("Status: Connected");
        status_label_->setStyleSheet("color: green; font-weight: bold;");
        port_selector_->setDisabled(true); // Lock selector while connected
        scan_btn_->setDisabled(true);
    } else {
        connect_btn_->setText("Connect");
        status_label_->setText("Status: Disconnected");
        status_label_->setStyleSheet("color: black;");
        port_selector_->setDisabled(false);
        scan_btn_->setDisabled(false);
    }
}

void MainWindow::append_data_to_console(const QByteArray &data) {
    // Optional: Convert raw bytes to String
    QString msg = QString::fromUtf8(data);

    // Timestamp for the log
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");

    console_output_->append(QString("[%1] %2").arg(timestamp, msg));
}