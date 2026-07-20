cmake_minimum_required(VERSION 3.16)

project(HockeySimulationEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find the required Qt components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# 2. Tell CMake to handle Qt features (MOC, UI, Resources) automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 3. Add all your source and header files here
add_executable(HockeySimulationEngine
    main.cpp
    player.hpp
    skater.hpp
    goalie.hpp
    team.hpp
    player.cpp
    skater.cpp
    goalie.cpp
    team.cpp
)

# 4. Link the Qt libraries to your executable
target_link_libraries(HockeySimulationEngine PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)