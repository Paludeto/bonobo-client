cmake_minimum_required(VERSION 3.16)

project(roboap-client LANGUAGES CXX)

add_subdirectory(proto)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt package
find_package(Qt6 COMPONENTS Core Network REQUIRED)

# Specify clients directory
set(CLIENTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/clients")

# List all client .cpp and .hh files recursively
file(GLOB_RECURSE CLIENTS_FILES "${CLIENTS_DIR}/*.cpp" "${CLIENTS_DIR}/*.h")

# Add executable with client files
add_executable(roboap-client main.cpp ${CLIENTS_FILES})

# Link the necessary libraries
target_link_libraries(roboap-client 
    Qt6::Core 
    Qt6::Network
    pb_msgs_lib
)

# Include clients directory
target_include_directories(roboap-client PRIVATE ${CLIENTS_DIR})