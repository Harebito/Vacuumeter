cmake_minimum_required(VERSION 3.16)
project(Vacuumeter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets SerialPort Network Sql)

qt_standard_project_setup()

qt_add_executable(appVacuumeter
        main.cpp
        main_window.cpp
        usb_transport.cpp
        wifi_transport.cpp
        protocol_handler.cpp
        database_manager.cpp

        transport_interface.h
        wifi_transport.h
        usb_transport.h
        protocol_handler.h
        main_window.h
        database_manager.h
)

target_link_libraries(appVacuumeter PRIVATE
        Qt6::Widgets
        Qt6::SerialPort
        Qt6::Network
        Qt6::Sql
)