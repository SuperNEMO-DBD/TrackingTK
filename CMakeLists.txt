#-----------------------------------------------------------------------
# Enforce an out-of-source build.
#
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(STATUS "TrackingTK requires an out-of-source build.")
  message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR} first:")
  message(STATUS "  CMakeCache.txt")
  message(STATUS "  CMakeFiles")
  message(STATUS "Once these files are removed, create a separate directory")
  message(STATUS "and run CMake from there, pointing it to:")
  message(STATUS "  ${CMAKE_SOURCE_DIR}")
  message(FATAL_ERROR "in-source build detected")
endif()

# Check cmake version meets our requirements
cmake_minimum_required(VERSION 3.3)

# Declare project, which will configure compiler for us
project(TrackingTK VERSION "0.1.0")

#Finds Falaise and ROOT packages and defines cmake variables
find_package(Falaise REQUIRED 5.1.1)
# find_package(ROOT REQUIRED)

include(GNUInstallDirs)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Bayeux_INCLUDE_DIRS}
  ${Falaise_INCLUDE_DIRS}
  )

set(TrackingTK_HEADERS
  sntk/datamodels.hpp
  sntk/geometry.hpp
  sntk/tracking_module.hpp
  sntk/notracking/no_tracking_driver.hpp
  sntk/notracking/working_data.hpp
  sntk/notracking/viewer.hpp
  )

set(TrackingTK_SOURCES
  sntk/datamodels.cpp
  sntk/geometry.cpp
  sntk/tracking_module.cpp
  sntk/notracking/no_tracking_driver.cpp
  sntk/notracking/working_data.cpp
  sntk/notracking/viewer.cpp
 )

add_library(TrackingTK SHARED
  ${TrackingTK_HEADERS}
  ${TrackingTK_SOURCES}
  )

target_link_libraries(TrackingTK
  PUBLIC
  Falaise::FalaiseModule
  # ${ROOT_LIBRARIES} ROOT::Geom
  )  

install(TARGETS TrackingTK DESTINATION ${CMAKE_INSTALL_PLUGINDIR})

configure_file(
  trackingtk-config.in
  ${PROJECT_BINARY_DIR}/trackingtk-config
  @ONLY)

install(PROGRAMS ${PROJECT_BINARY_DIR}/trackingtk-config
  DESTINATION ${CMAKE_INSTALL_BINDIR})

option(TrackingTK_ENABLE_TESTING "Build unit testing system for TrackingTK" ON)


if(TrackingTK_ENABLE_TESTING)
  enable_testing()
  # Build the test subdirectory
  add_subdirectory(test)
endif()


# - end
