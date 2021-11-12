cmake_minimum_required(VERSION 3.16)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

if(Qt6_FOUND) 
  message(STATUS "Creating Target Qt6::Core")
  set_target_properties(Qt::Core PROPERTIES IMPORTED_GLOBAL TRUE)
  message(STATUS "Creating Target Qt6::Core - done")
  message(STATUS "Creating Target Qt6::Widgets")
  set_target_properties(Qt::Widgets PROPERTIES IMPORTED_GLOBAL TRUE)
  message(STATUS "Creating Target Qt6::Widgets - done")
else()
  message(FATAL_ERROR "-- Qt6 not found")
endif()
