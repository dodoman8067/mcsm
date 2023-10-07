cmake_minimum_required(VERSION 3.16)
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

# Set compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto")
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("-- Detected compiler " ${CMAKE_CXX_COMPILER_ID} ". Using -static flags")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -flto")
    endif()
endif()

# Find dependencies 
find_package(CURL REQUIRED CONFIG)
find_package(nlohmann_json 3.2.0 REQUIRED)

# Add executable
add_executable(mcsm ${SRC_FILES} ${RESOURCE_FILE})

# Link dependencies
target_link_libraries(mcsm PRIVATE CURL::libcurl)
target_link_libraries(mcsm PRIVATE nlohmann_json::nlohmann_json)