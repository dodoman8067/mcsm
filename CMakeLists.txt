cmake_minimum_required(VERSION 3.24)
project(mcsm)
include(FetchContent)

# Detect OS
if(NOT LINUX AND NOT WIN32 AND NOT APPLE)
    message(FATAL_ERROR "This platform is not supported")
endif()

# Add options
option(BUILD_STATIC_EXE "Build static executable" OFF)
option(USE_SYSTEM_DEPS "Prefer system-installed libraries. Only affects nlohmann/json and tl::expected." ON)

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
# --- libcurl ---
find_package(CURL REQUIRED)
if(NOT CURL_FOUND)
    message(STATUS "libcurl not found system-wide; trying pkg-config")
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(CURL_PKG libcurl QUIET)
        if(CURL_PKG_FOUND)
            add_library(CURL::libcurl INTERFACE IMPORTED)
            set_target_properties(CURL::libcurl PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${CURL_PKG_INCLUDE_DIRS}"
                INTERFACE_LINK_LIBRARIES "${CURL_PKG_LIBRARIES}")
        endif()
    endif()
endif()
if(NOT TARGET CURL::libcurl)
    message(FATAL_ERROR "libcurl not found. install libcurl-dev or use a toolchain (vcpkg/conan).")
endif()

# --- nlohmann_json (header-only) ---
# prefer system if available, otherwise fetch pinned tag
if (USE_SYSTEM_DEPS)
    find_package(nlohmann_json REQUIRED) # provides nlohmann_json::nlohmann_json
endif()
if (NOT nlohmann_json_FOUND)
    FetchContent_Declare(nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

# --- tl::expected (header-only) ---
if (USE_SYSTEM_DEPS)
    find_package(tl-expected REQUIRED)
endif()
if(NOT tl-expected_FOUND)
    FetchContent_Declare(tl_expected
        GIT_REPOSITORY https://github.com/TartanLlama/expected.git
        GIT_TAG v1.3.1
    )
    FetchContent_GetProperties(tl_expected)
    if(NOT tl_expected_POPULATED)
        FetchContent_Populate(tl_expected)
        add_library(tl_expected INTERFACE)
        target_include_directories(tl_expected INTERFACE ${tl_expected_SOURCE_DIR}/include)
        add_library(tl::expected ALIAS tl_expected)
    endif()
    message("-- [mcsm] Fetched tl::expected to " ${tl_expected_SOURCE_DIR} ".")
endif()

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
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS_DEBUG "-g -DMCSM_DEBUG -fsanitize=address,undefined -fno-omit-frame-pointer -fsanitize-address-use-after-scope")
    elseif(APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        set(CMAKE_CXX_FLAGS_DEBUG "-g -DMCSM_DEBUG -fsanitize=address,undefined -fno-omit-frame-pointer")
    endif()
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
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
    elseif(APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-dead_strip")
    endif()
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
target_link_libraries(mcsm
  PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
    tl::expected
)

if(LINUX AND CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(mcsm PRIVATE Backward::Backward)
endif()