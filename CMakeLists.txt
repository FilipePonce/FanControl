cmake_minimum_required(VERSION 3.18)
project(FanControl)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find and include libmodbus
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMODBUS REQUIRED libmodbus)
include_directories(${LIBMODBUS_INCLUDE_DIRS})

# Find libcurl
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

# Set the output directories for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

# Add executable and link libraries
add_executable(FanControl main.cpp modbus_relay.cpp backend.cpp)

# Set output directory for FanControl executable
set_target_properties(FanControl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# Find Mosquitto
find_path(MOSQUITTO_INCLUDE_DIR mosquitto.h)
find_library(MOSQUITTO_LIBRARY mosquitto)

if (MOSQUITTO_INCLUDE_DIR AND MOSQUITTO_LIBRARY)
    message(STATUS "Found Mosquitto: ${MOSQUITTO_LIBRARY}")
    include_directories(${MOSQUITTO_INCLUDE_DIR})
else ()
    message(FATAL_ERROR "Could not find Mosquitto library and headers")
endif ()

# Link libraries
target_link_libraries(FanControl
    PRIVATE
    ${LIBMODBUS_LIBRARIES}
    ${CURL_LIBRARY}
    ${MOSQUITTO_LIBRARY}
    pthread
)

# Add the nlohmann/json library
add_subdirectory(external/json)

# Link the library to the FanControl target
target_link_libraries(FanControl PRIVATE nlohmann_json::nlohmann_json)

# Custom command to create a symlink to frontend files after build
add_custom_command(TARGET FanControl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/frontend ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/frontend
    COMMENT "Creating symlink to frontend directory"
)

# Install executable to /usr/local/bin or specified location
install(TARGETS FanControl
    DESTINATION /usr/local/bin
)
