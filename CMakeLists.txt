cmake_minimum_required(VERSION 3.16)

project(HockeySimulationEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the required Qt components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Handle Qt features automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Source and header files
add_executable(HockeySimulationEngine
    main.cpp
    makewindow.hpp
    player.hpp
    skater.hpp
    goalie.hpp
    team.hpp
    makewindow.cpp
    player.cpp
    skater.cpp
    goalie.cpp
    team.cpp
)

# Link Qt libraries to executable
target_link_libraries(HockeySimulationEngine PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)