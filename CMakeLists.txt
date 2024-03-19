# Check cmake version meets our requirements
cmake_minimum_required(VERSION 3.3)

# Declare project, which will configure compiler for us
project(TrackinTK)

#Finds Falaise and ROOT packages and defines cmake variables
find_package(Falaise REQUIRED)
# find_package(ROOT    REQUIRED)

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

target_link_libraries(TrackingTK PUBLIC
  Falaise::Falaise
  # ${ROOT_LIBRARIES} ROOT::Geom
  )  

install(TARGETS TrackingTK DESTINATION ${CMAKE_INSTALL_PLUGINDIR})

option(TrackingTK_ENABLE_TESTING "Build unit testing system for TrackingTK" ON)


if(TrackingTK_ENABLE_TESTING)
  enable_testing()
  # Build the test subdirectory
  add_subdirectory(test)
endif()


# - end
