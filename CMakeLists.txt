cmake_minimum_required(VERSION 3.22)
project(RandomWalkVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Charts)

add_executable(RandomWalkVisualizer
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/widgets/ChartWidget.cpp
    src/widgets/ChartWidget.h
)

target_link_libraries(RandomWalkVisualizer
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Charts
)
