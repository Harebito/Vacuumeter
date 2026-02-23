#ifndef USBMONITOR_MAIN_WINDOW_H
#define USBMONITOR_MAIN_WINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>

#include "database_manager.h"
#include "usb_transport.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handle_scan();
    void handle_connect();
    void update_connection_state(bool is_connected);
    void append_data_to_console(const QByteArray &data);

private:
    void setup_ui();
    void setup_menu();

    // Logic Backend
    DeviceManager *device_manager_;

    // UI Components
    QWidget *central_widget_;
    QVBoxLayout *main_layout_;

    QComboBox *port_selector_;
    QPushButton *scan_btn_;
    QPushButton *connect_btn_;
    QLabel *status_label_;
    QTextEdit *console_output_; // To display incoming USB data

    // DB Manager
    DatabaseManager *db_manager_;


};
#endif // USBMONITOR_MAIN_WINDOW_H