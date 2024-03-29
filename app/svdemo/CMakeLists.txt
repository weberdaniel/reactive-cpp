cmake_minimum_required( VERSION 3.12 )

message(STATUS "-- Creating Target svdemo: Try to find CAF ...")

# only use the specific installation of CAF in external/ folder
# disable all other search locations

find_package(Unwind REQUIRED)
if(NOT DOCKER)
  message(STATUS "-- Creating Target svdemo: Docker NOT used: Locating CAF somewhere: ...")
  ExternalProject_Get_Property(actorframework binary_dir)
  message(STATUS "-- Creating Target svdemo: Docker NOT used: Locating CAF in external/caf: ...")
  find_package(CAF COMPONENTS core io
                   CONFIG
                   NO_PACKAGE_ROOT_PATH
                   NO_CMAKE_PATH
                   NO_CMAKE_ENVIRONMENT_PATH
                   NO_SYSTEM_ENVIRONMENT_PATH
                   NO_CMAKE_SYSTEM_PATH
                   NO_CMAKE_PACKAGE_REGISTRY
                   NO_CMAKE_SYSTEM_PACKAGE_REGISTRY
                   HINTS ${binary_dir} )
  if(CAF_FOUND)
    message(STATUS "-- Creating Target svdemo: Docker NOT used: CAF found")
  else() 
    message(STATUS "-- Creating Target svdemo: Docker NOT used: CAF not found")
  endif()
endif()

if(DOCKER) 
  message(STATUS "-- Creating Target svdemo: Docker used: Locating CAF somewhere: ...")
  message(STATUS "-- Creating Target svdemo: Docker used: set rootdir /project/actor-framework/build: ...")
  set(CAF_ROOT_DIR "/project/actor-framework/build")
  find_package(CAF COMPONENTS core io
                   CONFIG
                   NO_PACKAGE_ROOT_PATH
                   NO_CMAKE_PATH
                   NO_CMAKE_ENVIRONMENT_PATH
                   NO_SYSTEM_ENVIRONMENT_PATH
                   NO_CMAKE_SYSTEM_PATH
                   NO_CMAKE_PACKAGE_REGISTRY
                   NO_CMAKE_SYSTEM_PACKAGE_REGISTRY
		   HINTS ${CAF_ROOT_DIR} )
  if(CAF_FOUND)
    message(STATUS "-- Creating Target svdemo: Docker used: CAF found")
  else() 
    message(FATAL_ERROR "-- Creating Target svdemo: Docker used: CAF not found")
  endif()
endif()


if(CAF_FOUND)
	message(STATUS "-- Creating Target svdemo: CAF found ...")
  project( svdemo
          VERSION 0.1.0
          DESCRIPTION "A simple test")

  add_executable(svdemo
  svdemo/supervisor.h 
  svdemo/worker.h 
  svdemo/messages.h 
  svdemo/types.h 
  src/worker.cpp
  src/main.cpp
  src/supervisor.cpp)

  set(CAF_LOG_LEVEL TRACE)

  target_compile_features(svdemo PRIVATE cxx_std_17)
  target_include_directories(svdemo PRIVATE
          ${CMAKE_CURRENT_BINARY_DIR}
          ${CMAKE_CURRENT_LIST_DIR} )
  if(Unwind_FOUND)
    get_property(Unwind_LIBRARIES TARGET unwind::unwind PROPERTY INTERFACE_LINK_LIBRARIES)
    get_property(Unwind_INCLUDE_DIR TARGET unwind::unwind PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
    message("-- Unwind was found: ${Unwind_INCLUDE_DIR}")
    message("-- Unwind was found: ${Unwind_LIBRARIES}")
    target_link_libraries(svdemo PUBLIC unwind::unwind)
  else()
    message("-- Unwind was not found ")
  endif()

  message("-- Creating Target svdemo: link CAF ...")
  set_target_properties(CAF::internal PROPERTIES IMPORTED_GLOBAL TRUE)
  set_target_properties(CAF::core PROPERTIES IMPORTED_GLOBAL TRUE)
  set_target_properties(CAF::io PROPERTIES IMPORTED_GLOBAL TRUE)

  target_link_libraries(svdemo PRIVATE CAF::core CAF::io)
else()
  message("-- Creating Target svdemo: CAF not found!")
endif()
