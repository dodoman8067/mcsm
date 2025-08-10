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
find_package(CURL REQUIRED CONFIG)
find_package(nlohmann_json REQUIRED)
find_package(tl-expected REQUIRED)
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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -DMCSM_NLOHMANN_VER=\\\"${nlohmann_json_VERSION}\\\"")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -DMCSM_DEBUG -fsanitize=address,undefined -fno-omit-frame-pointer -fsanitize-address-use-after-scope")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffunction-sections -fdata-sections")
endif()

if(BUILD_STATIC_EXE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("-- Detected compiler " ${CMAKE_CXX_COMPILER_ID} ". Using -static flags")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    endif()
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -DMCSM_DEBUG -fsanitize=address,undefined -D_GLIBCXX_ASSERTIONS")
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
endif()

# Add executable
if(LINUX AND CMAKE_BUILD_TYPE MATCHES Debug)
    add_executable(mcsm ${SRC_FILES} ${RESOURCE_FILE} backward/backward.cpp)
else()
    add_executable(mcsm ${SRC_FILES} ${RESOURCE_FILE})
endif()

# --- Post-build: strip (only in Release) ---
if(CMAKE_BUILD_TYPE MATCHES Release)
    if(MINGW)
        find_program(STRIP_TOOL NAMES strip.exe)
    elseif(UNIX)
        find_program(STRIP_TOOL NAMES strip)
    endif()

    if(STRIP_TOOL)
        message("-- Found strip: ${STRIP_TOOL}")
        add_custom_command(TARGET mcsm POST_BUILD
            COMMAND ${STRIP_TOOL} $<TARGET_FILE:mcsm>
            COMMENT "Stripping symbols to reduce binary size"
        )
    endif()
endif()

# Link dependencies
target_link_libraries(mcsm PRIVATE CURL::libcurl)
target_link_libraries(mcsm PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(mcsm PRIVATE tl::expected)

# Some dependencies for libgit2 in Windows
if(WIN32)
    target_link_libraries(mcsm PRIVATE Winhttp)
    target_link_libraries(mcsm PRIVATE Rpcrt4)
endif()

target_link_libraries(mcsm PRIVATE ${LIBGIT2_LIBRARIES})

if(LINUX AND CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(mcsm PRIVATE Backward::Backward)
endif()