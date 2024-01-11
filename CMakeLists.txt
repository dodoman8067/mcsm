cmake_minimum_required(VERSION 3.24)
project(mcsm)

# Detect OS
if(NOT LINUX AND NOT WIN32)
    message(FATAL_ERROR "This platform is not supported")
endif()

# Add options
option(BUILD_STATIC_EXE "Build static executable" OFF)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set dependency options
if(BUILD_STATIC_EXE)
    set(ZLIB_USE_STATIC_LIBS TRUE)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
    set(LIBGIT2_USE_STATIC_LIBS TRUE)
endif()
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

# Find dependencies 
find_package(termcolor REQUIRED)
find_package(CURL REQUIRED CONFIG)
find_package(nlohmann_json 3.2.0 REQUIRED)
include(Findlibgit2)
if(LINUX AND CMAKE_BUILD_TYPE MATCHES Debug)
    find_package(Backward REQUIRED)
endif()

# Set include directory
include_directories(${CMAKE_SOURCE_DIR}/include ${LIBGIT2_INCLUDE_DIR})

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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto")
endif()

if(BUILD_STATIC_EXE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("-- Detected compiler " ${CMAKE_CXX_COMPILER_ID} ". Using -static flags")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    endif()
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
endif()

# Add executable
if(LINUX AND CMAKE_BUILD_TYPE MATCHES Debug)
    add_executable(mcsm ${SRC_FILES} ${RESOURCE_FILE} backward/backward.cpp)
else()
    add_executable(mcsm ${SRC_FILES} ${RESOURCE_FILE})
endif()

# Link dependencies
target_link_libraries(mcsm PRIVATE CURL::libcurl)
target_link_libraries(mcsm PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(mcsm PRIVATE termcolor::termcolor)
target_link_libraries(mcsm PRIVATE ${LIBGIT2_LIBRARIES})
if(LINUX AND CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(mcsm PRIVATE Backward::Backward)
endif()