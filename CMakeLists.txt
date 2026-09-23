cmake_minimum_required(VERSION 3.16)

project(TraficSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(TraficSimulator
    src/main.cpp
    src/Vector2D.cpp
    src/Car.cpp
    src/Road.cpp
    src/RoadSign.cpp
    src/Crossroad.cpp
    src/TrafficLight.cpp
    src/Route.cpp
    src/Simulation.cpp
    src/Graphics.cpp
)

target_include_directories(TraficSimulator PRIVATE src)

target_link_libraries(TraficSimulator PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)