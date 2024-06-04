cmake_minimum_required(VERSION 3.10)

project(RaylibProject)

# Specify the source files
set(SOURCES
    src/config.c
    src/controller.c
    src/list.c
    src/view.c
    src/model.c
)

# Include the cmake directory where Findraylib.cmake is located
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Find Raylib
find_package(raylib REQUIRED)

# Define the executable
add_executable(DroneSimulator ${SOURCES})

# Link Raylib and other required libraries
target_link_libraries(DroneSimulator PRIVATE ${raylib_LIBRARIES} m)

# Include Raylib directories
target_include_directories(DroneSimulator PRIVATE ${raylib_INCLUDE_DIRS})

# Special settings for Windows
if(WIN32)
    target_link_libraries(DroneSimulator PRIVATE winmm gdi32)
endif()

# Debug configuration
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

# Add custom targets for running and debugging
add_custom_target(run
    COMMAND ./DroneSimulator
    DEPENDS DroneSimulator
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -E env LLDB_TERMINAL=1 lldb ./DroneSimulator
    DEPENDS DroneSimulator
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)