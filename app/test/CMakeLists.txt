# only use the specific installation of CAF in external/ folder
# disable all other search locations
cmake_minimum_required(VERSION 3.12)

message("-- Creating Target testit: Try to find CAF ...")

if(NOT DOCKER)
  message(STATUS "-- Creating Target testit: Docker NOT used: Locating CAF somewhere: ...")
  if(TARGET actorframework)
    message(STATUS "-- Creating Target testit: Docker NOT used: Found target actorframework ...")
    ExternalProject_Get_Property(actorframework binary_dir)
    if(NOT TARGET CAF::core)
      find_package(CAF COMPONENTS internal core io
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
        message(STATUS "-- Creating Target testit: Docker NOT used: CAF found")
      else()
        message(STATUS "-- Creating Target testit: Docker NOT used: CAF not found")
      endif()
    endif()
  else()
    message(WARNING 
      "-- Creating Target testit: Docker NOT used: Target actorframework does not exist ...")
  endif()
endif()

if(DOCKER)
  message("-- Creating Target testit: Docker used: Try to find CAF via find_package ...")
  message("-- Creating Target testit: Docker used: set rootdir /project/actor-framework/build: ...")
  set(CAF_ROOT_DIR "/project/actor-framework/build")
  if(NOT TARGET CAF::core)
    find_package(CAF COMPONENTS internal core io
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
      message(STATUS "-- Creating Target testit: Docker NOT used: CAF found")
    else()
      message(FATAL_ERROR "-- Creating Target testit: Docker NOT used: CAF not found")
    endif()
  endif()
endif()

if(TARGET CAF::core)
  message("-- Creating Target testit: ... found it! ")
  project( testit
           VERSION 0.1.0
           DESCRIPTION "A simple test")

  find_package(Qt5 COMPONENTS Core Gui Widgets)
  find_package(Unwind REQUIRED)

  if(Qt5_FOUND)
    qt5_wrap_ui(GROUP_CHAT_UI_HDR chatwindow.ui)
    qt5_wrap_cpp(GROUP_CHAT_MOC_SRC chatwidget.hpp)

    add_executable(testqt
      qt_group_chat.cpp
      chatwidget.cpp
      ${GROUP_CHAT_MOC_SRC}
      ${GROUP_CHAT_UI_HDR} )

    target_link_libraries(testqt
      PRIVATE Qt::Core
      PRIVATE Qt::Gui
      PRIVATE Qt::Widgets
      PRIVATE CAF::core
      PRIVATE CAF::io
    )

    target_compile_features(testqt PRIVATE cxx_std_17)
    target_include_directories(testqt PRIVATE
                               ${CMAKE_CURRENT_BINARY_DIR}
                               ${CMAKE_CURRENT_LIST_DIR} )
  endif()

# add_executable(testpqxx
#   libpqxx_test.cpp)
# target_include_directories(testpqxx PRIVATE
#                            ${CMAKE_CURRENT_BINARY_DIR}
#                            ${CMAKE_CURRENT_LIST_DIR} )
# target_compile_features(testpqxx PRIVATE cxx_std_17)
  if(NOT DOCKER)
    message(STATUS "-- Creating Target testit: ... you need to have a libpqxx installed .... ")
    target_link_libraries(testpqxx -lpqxx -lpq)
  else()
#   ExternalProject_Get_Property(libpqxx binary_dir)
#   ExternalProject_Get_Property(libpqxx source_dir)
#   find_library(PQXX_LIB NAMES pqxx
#                  HINTS "${binary_dir}/src/.libs"
#		  HINTS "${binary_dir}"
#		  HINTS "src"
#		  HINTS ".libs"
#                  HINTS "${source_dir}/src/.libs"
#		  HINTS "${source_dir}"
#		  HINTS "src"
#		  HINTS ".libs")
#   file(GLOB LS *)
#   message(STATUS "${LS}")
#                                     
#   target_link_libraries(testpqxx PRIVATE ${PQXX_LIB})
  endif()

endif()
