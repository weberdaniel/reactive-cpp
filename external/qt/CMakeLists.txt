cmake_minimum_required(VERSION 3.16)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

if(Qt6_FOUND) 
  set_target_properties(Qt::Core PROPERTIES IMPORTED_GLOBAL TRUE)
  set_target_properties(Qt::Widgets PROPERTIES IMPORTED_GLOBAL TRUE)
endif()
