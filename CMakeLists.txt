cmake_minimum_required(VERSION 3.16)
project(RandomWalkViewer LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(RandomWalkViewer
    src/main.cpp
    src/MainWindow.cpp
    src/FileListWidget.cpp
    src/AnimationWidget.cpp
)

target_include_directories(RandomWalkViewer PRIVATE src)
target_link_libraries(RandomWalkViewer PRIVATE Qt6::Widgets)
