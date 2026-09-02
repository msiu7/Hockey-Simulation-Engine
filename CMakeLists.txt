cmake_minimum_required(VERSION 3.16)

project(HockeySimulationEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the required Qt components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets QuickWidgets)

# Handle Qt features automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Source and header files
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
    mainwindow.hpp
    mainwindow.cpp
    mainwindow.ui
    include/spacer.hpp
    include/dasmig/namegen.hpp
    include/dasmig/random.hpp
    rink.hpp
    rink.cpp
    singleGameSimulation.cpp
    singleGameSimulation.hpp
    rinkView.qml

)

# Link Qt libraries to executable
target_link_libraries(HockeySimulationEngine PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::QuickWidgets)

# double heap space
if(MSVC)
    add_compile_options(/Zm200)
endif()

# including custom downloaded libraries (in the include folder)
target_include_directories(HockeySimulationEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Recursively grab every file inside the resources directory
file(GLOB_RECURSE RESOURCE_FILES "resources/*")

# Add those files to executable target so Qt Creator displays the folder
target_sources(HockeySimulationEngine PRIVATE ${RESOURCE_FILES})

# Copy the entire resources directory to where the executable lives (necessary? let's see later)
add_custom_command(TARGET HockeySimulationEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:HockeySimulationEngine>/resources
)