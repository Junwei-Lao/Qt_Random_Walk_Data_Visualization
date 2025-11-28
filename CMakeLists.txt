cmake_minimum_required(VERSION 3.16)
project(QtFirstApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

add_executable(QtFirstApp
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/widgets/SidebarWidget.cpp
    src/widgets/SidebarWidget.h
    src/widgets/PlotWidget.cpp
    src/widgets/PlotWidget.h
)

target_link_libraries(QtFirstApp PRIVATE Qt6::Widgets Qt6::Charts)
