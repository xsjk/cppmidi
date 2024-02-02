# 2018 (c) Juan G. Victores, Bartek Łukawski, Stephen Sinclair
# CopyPolicy: RtMidi license.

# Set minimum CMake required version for this project.
cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

# Define a C++ project.
project(RtMidi LANGUAGES CXX C)

# standards version
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Check for Jack (any OS)
find_library(JACK_LIB jack)
find_package(PkgConfig)
pkg_check_modules(jack jack)
if(JACK_LIB OR jack_FOUND)
  set(HAVE_JACK TRUE)
endif()

# Necessary for Windows
if(WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Standard CMake options
option(BUILD_SHARED_LIBS "Build as shared library" ON)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")
endif()
if(WINDOWS)
  set(CMAKE_DEBUG_POSTFIX d CACHE STRING "Postfix for debug version of library")
endif()

# Build Options
set(RTMIDI_TARGETNAME_UNINSTALL "uninstall" CACHE STRING "Name of 'uninstall' build target")

# API Options
option(RTMIDI_API_JACK "Compile with JACK support." ${HAVE_JACK})
option(RTMIDI_API_WINMM "Compile with WINMM support." ${WIN32})
option(RTMIDI_API_CORE "Compile with CoreMIDI support." ${APPLE})
option(RTMIDI_API_ALSA "Compile with ALSA support." ${ALSA})
option(RTMIDI_API_AMIDI "Compile with Android support." ${ANDROID})

# Add -Wall if possible
if (CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif (CMAKE_COMPILER_IS_GNUCXX)

# Add debug flags
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-D__RTMIDI_DEBUG__)
  if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif (CMAKE_COMPILER_IS_GNUCXX)
endif ()

# # Read libtool version info from configure.ac
# set(R "m4_define\\(\\[lt_([a-z]+)\\], ([0-9]+)\\)")
# file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/configure.ac" CONFIGAC
#   REGEX ${R})
# foreach(_S ${CONFIGAC})
#   string(REGEX REPLACE ${R} "\\1" k ${_S})
#   string(REGEX REPLACE ${R} "\\2" v ${_S})
#   set(SO_${k} ${v})
# endforeach()
# math(EXPR SO_current_minus_age "${SO_current} - ${SO_age}")
# set(SO_VER "${SO_current_minus_age}")
# set(FULL_VER "${SO_current_minus_age}.${SO_age}.${SO_revision}")

# # Read package version info from configure.ac
# set(R "AC_INIT\\(RtMidi, ([0-9\\.]+),.*\\)")
# file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/configure.ac" CONFIGAC
#   REGEX ${R})
# string(REGEX REPLACE ${R} "\\1" PACKAGE_VERSION ${CONFIGAC})

# Init variables
set(rtmidi_SOURCES RtMidi.cpp RtMidi.h)
set(LINKLIBS)
set(PUBLICLINKLIBS)
set(INCDIRS)
set(PKGCONFIG_REQUIRES)
set(LIBS_REQUIRES)
set(API_DEFS)
set(API_LIST)
set(PACKAGE_DEPENDENCIES)

# Tweak API-specific configuration.

# Jack
if(RTMIDI_API_JACK)
  if (NOT HAVE_JACK)
    message(FATAL_ERROR "Jack API requested but no Jack dev libraries found")
  endif()
  set(NEED_PTHREAD ON)
  list(APPEND PKGCONFIG_REQUIRES "jack")
  list(APPEND API_DEFS "-D__UNIX_JACK__")
  list(APPEND API_LIST "jack")
  if(jack_FOUND)
    list(APPEND LINKLIBS ${jack_LIBRARIES})
    list(APPEND INCDIRS ${jack_INCLUDEDIR})
  else()
    list(APPEND LINKLIBS ${JACK_LIB})
  endif()

  # Check for jack_port_rename
  include(CheckSymbolExists)
  set(tmp_CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES})
  list(APPEND CMAKE_REQUIRED_LIBRARIES jack)
  check_symbol_exists(jack_port_rename ${jack_INCLUDEDIR}/jack/jack.h JACK_HAS_PORT_RENAME)
  set(CMAKE_REQUIRED_LIBRARIES ${tmp_CMAKE_REQUIRED_LIBRARIES})
  if(JACK_HAS_PORT_RENAME)
    list(APPEND API_DEFS "-DJACK_HAS_PORT_RENAME")
  endif()
endif()

# ALSA
find_package(ALSA)
if(ALSA_FOUND OR ALSA)
  set(NEED_PTHREAD ON)
  list(APPEND INCDIRS ${ALSA_INCLUDE_DIR})
  list(APPEND LINKLIBS ALSA::ALSA)
  list(APPEND PKGCONFIG_REQUIRES "alsa")
  list(APPEND API_DEFS "-D__LINUX_ALSA__")
  list(APPEND API_LIST "alsa")
  list(APPEND PACKAGE_DEPENDENCIES "find_dependency(ALSA)")
endif()

# WinMM
if(RTMIDI_API_WINMM)
  list(APPEND API_DEFS "-D__WINDOWS_MM__")
  list(APPEND API_LIST "winmm")
  list(APPEND LINKLIBS winmm)
endif()

# CoreMIDI
if(RTMIDI_API_CORE)
  find_library(CORESERVICES_LIB CoreServices)
  find_library(COREAUDIO_LIB CoreAudio)
  find_library(COREMIDI_LIB CoreMIDI)
  find_library(COREFOUNDATION_LIB CoreFoundation)
  list(APPEND API_DEFS "-D__MACOSX_CORE__")
  list(APPEND API_LIST "coremidi")
  list(APPEND LINKLIBS ${CORESERVICES_LIB} ${COREAUDIO_LIB} ${COREMIDI_LIB} ${COREFOUNDATION_LIB})
  list(APPEND LIBS_REQUIRES "-framework CoreServices -framework CoreAudio -framework CoreMIDI -framework CoreFoundation")
  list(APPEND LINKFLAGS "-Wl,-F/Library/Frameworks")
endif()

# Android AMIDI
if(ANDROID)
  set(NEED_PTHREAD ON)
  set(JAVA_INCLUDE_PATH2 NotNeeded)
  set(JAVA_AWT_INCLUDE_PATH NotNeeded)
  find_package(JNI)
#  find_library(ALOG_LIB log android)
  list(APPEND API_DEFS "-D__AMIDI__")
  list(APPEND API_LIST "amidi")
  list(APPEND LINKLIBS log ${JNI_LIBRARIES} amidi)
endif()

# pthread
if (NEED_PTHREAD)
  find_package(Threads REQUIRED
    CMAKE_THREAD_PREFER_PTHREAD
    THREADS_PREFER_PTHREAD_FLAG)
  list(APPEND PUBLICLINKLIBS Threads::Threads)
  list(APPEND PACKAGE_DEPENDENCIES "find_dependency(Threads)")
endif()

# Create library targets.
set(LIB_TARGETS)

# Use RTMIDI_BUILD_SHARED_LIBS / RTMIDI_BUILD_STATIC_LIBS if they
# are defined, otherwise default to standard BUILD_SHARED_LIBS.
if (DEFINED RTMIDI_BUILD_SHARED_LIBS AND NOT RTMIDI_BUILD_SHARED_LIBS STREQUAL "")
  if (RTMIDI_BUILD_SHARED_LIBS)
    add_library(rtmidi SHARED ${rtmidi_SOURCES})
  else()
    add_library(rtmidi STATIC ${rtmidi_SOURCES})
  endif()
elseif (DEFINED RTMIDI_BUILD_STATIC_LIBS AND NOT RTMIDI_BUILD_STATIC_LIBS STREQUAL "")
  if (RTMIDI_BUILD_STATIC_LIBS)
    add_library(rtmidi STATIC ${rtmidi_SOURCES})
  else()
    add_library(rtmidi SHARED ${rtmidi_SOURCES})
  endif()
else()
  add_library(rtmidi ${rtmidi_SOURCES})
endif()
list(APPEND LIB_TARGETS rtmidi)

# Add headers destination for install rule.
set_property(TARGET rtmidi PROPERTY PUBLIC_HEADER RtMidi.h)
set_target_properties(rtmidi PROPERTIES
  SOVERSION ${SO_VER}
  VERSION ${FULL_VER})

# Set include paths, populate target interface.
target_include_directories(rtmidi PRIVATE ${INCDIRS}
                                  PUBLIC
                                    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Set compile-time definitions
target_compile_definitions(rtmidi PRIVATE ${API_DEFS})
target_compile_definitions(rtmidi PRIVATE RTMIDI_EXPORT)
target_link_libraries(rtmidi PUBLIC ${PUBLICLINKLIBS} 
                             PRIVATE ${LINKLIBS})

