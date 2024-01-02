# - Try to find the libgit2 library
# Once done this will define
#
#  LIBGIT2_FOUND - System has libgit2
#  LIBGIT2_INCLUDE_DIR - The libgit2 include directory
#  LIBGIT2_LIBRARIES - The libraries needed to use libgit2
#  LIBGIT2_DEFINITIONS - Compiler switches required for using libgit2


# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
#FIND_PACKAGE(PkgConfig)
#PKG_SEARCH_MODULE(PC_LIBGIT2 libgit2)

# Option for using static libraries
OPTION(LIBGIT2_USE_STATIC_LIBS "Use static libraries for libgit2" OFF)

# Set up package configuration
FIND_PACKAGE(PkgConfig)
PKG_SEARCH_MODULE(PC_LIBGIT2 libgit2)

SET(LIBGIT2_DEFINITIONS ${PC_LIBGIT2_CFLAGS_OTHER})

# Find include directory
FIND_PATH(LIBGIT2_INCLUDE_DIR NAMES git2.h
   HINTS
   ${PC_LIBGIT2_INCLUDEDIR}
   ${PC_LIBGIT2_INCLUDE_DIRS}
)

# Conditional logic for static or shared libraries
IF(LIBGIT2_USE_STATIC_LIBS)
    SET(LIBGIT2_FIND_LIBRARY_SUFFIXES ".a")
ELSE()
    # Detect shared library suffix based on platform
    IF(WIN32)
        SET(LIBGIT2_FIND_LIBRARY_SUFFIXES ".dll.a")  # For MinGW
    ELSE()
        SET(LIBGIT2_FIND_LIBRARY_SUFFIXES ".so")     # For UNIX-like systems
    ENDIF()
ENDIF()

SET(CMAKE_FIND_LIBRARY_SUFFIXES_BACKUP ${CMAKE_FIND_LIBRARY_SUFFIXES})
SET(CMAKE_FIND_LIBRARY_SUFFIXES ${LIBGIT2_FIND_LIBRARY_SUFFIXES})

FIND_LIBRARY(LIBGIT2_LIBRARIES NAMES git2
   HINTS
   ${PC_LIBGIT2_LIBDIR}
   ${PC_LIBGIT2_LIBRARY_DIRS}
)

# Reset library suffixes after search
SET(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES_BACKUP})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(libgit2 DEFAULT_MSG LIBGIT2_LIBRARIES LIBGIT2_INCLUDE_DIR)

MARK_AS_ADVANCED(LIBGIT2_INCLUDE_DIR LIBGIT2_LIBRARIES)