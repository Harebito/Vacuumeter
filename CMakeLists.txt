cmake_minimum_required(VERSION 3.16)
project(UsbMonitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets SerialPort Network Sql)

qt_standard_project_setup()

qt_add_executable(appUsbMonitor
        main.cpp
        main_window.cpp
        usb_transport.cpp
        wifi_transport.cpp    # Don't forget this!
        protocol_handler.cpp  # And this!
        database_manager.cpp

        # Headers are optional here but good for IDEs
        transport_interface.h
        wifi_transport.h
        usb_transport.h
        protocol_handler.h
        main_window.h
        database_manager.h
)

target_link_libraries(appUsbMonitor PRIVATE
        Qt6::Widgets
        Qt6::SerialPort
        Qt6::Network
        Qt6::Sql
)