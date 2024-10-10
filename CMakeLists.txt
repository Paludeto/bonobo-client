cmake_minimum_required(VERSION 3.16)

project(roboap-client LANGUAGES CXX)

# Set basic CMake configurations
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt package
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Qt6Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

# Find Protobuf
find_package(Protobuf REQUIRED)

# Specify directories
set(PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
set(CLIENTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/clients")

# Include directories
include_directories(${PROTO_DIR} ${CLIENTS_DIR})

# List all the generated .pb.cc and .pb.h files
file(GLOB GENERATED_SOURCES "${PROTO_DIR}/*.cc")
file(GLOB GENERATED_HEADERS "${PROTO_DIR}/*.hh")

# List all client .cpp and .hh files recursively (including subdirectories like actuator/)
file(GLOB_RECURSE CLIENTS_FILES "${CLIENTS_DIR}/*.cpp" "${CLIENTS_DIR}/*.hh")

# Add executable with source files and generated files
add_executable(roboap-client
    main.cpp
    ${GENERATED_SOURCES}
    ${GENERATED_HEADERS}
    ${CLIENTS_FILES}
)

# Link the necessary libraries
target_link_libraries(roboap-client
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    ${Protobuf_LIBRARIES}
)

# Add custom target to show the clients directory in Qt Creator
add_custom_target(show_clients_files ALL SOURCES ${CLIENTS_FILES})

# Installation settings
include(GNUInstallDirs)
install(TARGETS roboap-client
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
