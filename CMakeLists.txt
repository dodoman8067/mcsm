cmake_minimum_required(VERSION 3.5)
project(mcsm)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Set resources.rc when win32
if(WIN32)
    set(RESOURCE_FILE ${CMAKE_SOURCE_DIR}/assets/resources.rc)
endif()

# Add CURL library
find_package(CURL REQUIRED CONFIG)

# Add executable
add_executable(mcsm ${SRC_FILES} ${RESOURCE_FILE})

# Link CURL library
target_link_libraries(mcsm PRIVATE CURL::libcurl)