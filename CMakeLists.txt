cmake_minimum_required(VERSION 3.5)
project(mcsm)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Source files
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Set resources.rc when win32
if(WIN32)
    set(RESOURCE_FILE ${CMAKE_SOURCE_DIR}/assets/resources.rc)
endif()

# Set build types when no CMAKE_BUILD_TYPE defined
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Sets compiler options
if(GCC OR MINGW OR CLANG)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()

# Find dependencies
find_package(CURL REQUIRED CONFIG)
find_package(nlohmann_json 3.2.0 REQUIRED)

# Add executable
add_executable(mcsm ${SRC_FILES} ${RESOURCE_FILE})

# Link dependencies
target_link_libraries(mcsm PRIVATE CURL::libcurl)
target_link_libraries(mcsm PRIVATE nlohmann_json::nlohmann_json)