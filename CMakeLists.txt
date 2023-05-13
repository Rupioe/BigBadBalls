cmake_minimum_required(VERSION 3.24)
project(bigbadballs)

#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_FLAGS "-no-pie")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        REQUIRED)

add_executable(bigbadballs main.cpp MainScene.cpp MainWindow.cpp Player.cpp medias.qrc Mutes.cpp Mutes.h Arrows.cpp Arrows.h)
target_link_libraries(bigbadballs
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Multimedia
        )
