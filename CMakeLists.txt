cmake_minimum_required(VERSION 3.16)

project(roboap-client LANGUAGES CXX)

add_subdirectory(proto)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt package
find_package(Qt6 COMPONENTS Core Network Gui REQUIRED)

# Specify clients directory
set(ENTITIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/entities")
set(CLIENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/client")
set(UTILS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/utils")

# List all client .cpp and .hh files recursively
file(GLOB_RECURSE ENTITIES_FILES "${ENTITIES_DIR}/*.cpp" "${ENTITIES_DIR}/*.h")
file(GLOB_RECURSE UTILS_FILES "${UTILS_DIR}/*.cpp" "${UTILS_DIR}/*.h")
file(GLOB_RECURSE CLIENT_FILES "${CLIENT_DIR}/*.cpp" "${CLIENT_DIR}/*.h")


# Add executable with client files
add_executable(roboap-client main.cpp ${ENTITIES_FILES} ${UTILS_FILES} ${CLIENT_FILES})

# Link the necessary libraries
target_link_libraries(roboap-client 
    Qt6::Core 
    Qt6::Network
    Qt6::Gui
    pb_msgs_lib
)

# Include clients directory
target_include_directories(roboap-client PRIVATE ${ENTITIES_DIR} ${UTILS_DIR} ${CLIENT_DIR})