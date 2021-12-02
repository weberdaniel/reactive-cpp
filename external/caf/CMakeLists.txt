cmake_minimum_required(VERSION 3.14)

project(external.caf)

set(VERSION 0.18.5)
set(DESCRIPTION "CAF")

include(ExternalProject)

# add all local cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)


if(docker)
  if(DEFINED ENV{CAF_ROOT})
    message(STATUS "set CAF_ROOT_DIR: $ENV{CAF_ROOT}")
    set(CAF_ROOT_DIR $ENV{CAF_ROOT})
  endif()
  find_package(CAF COMPONENTS core io)
  if(CAF_FOUND)
    message(STATUS "-- Creating Target ActorFramework")
    add_library(ActorFramework INTERFACE IMPORTED GLOBAL)
    target_include_directories(ActorFramework INTERFACE ${CAF_INCLUDE_DIRS})
    target_link_libraries(ActorFramework INTERFACE ${CAF_LIBRARIES})
    target_compile_features(ActorFramework INTERFACE cxx_std_14)
    message(STATUS "-- Creating Target ActorFramework - done")
  else()
    message(FATAL_ERROR "-- ActorFramework not found")
  endif()
else()
  if(CAF)
    message("-- Creating Target ActorFramework")
    # add the external Cmake project 
    ExternalProject_Add(actorframework URL ${CMAKE_CURRENT_LIST_DIR}/actor-framework
      UPDATE_COMMAND ./configure
      INSTALL_COMMAND cmake -E echo "-- Skip install step.")
    ExternalProject_Get_Property(actorframework source_dir)
    ExternalProject_Get_Property(actorframework binary_dir)
    # Create dirs of the external project
    file(MAKE_DIRECTORY ${source_dir})
    file(MAKE_DIRECTORY ${source_dir}/libcaf_core)
    file(MAKE_DIRECTORY ${source_dir}/libcaf_core/caf)
    file(MAKE_DIRECTORY ${source_dir}/libcaf_core/caf/detail)
    file(MAKE_DIRECTORY ${source_dir}/libcaf_io)
    file(MAKE_DIRECTORY ${source_dir}/libcaf_openssl)
    file(MAKE_DIRECTORY ${source_dir}/libcaf_test)

    file(MAKE_DIRECTORY ${binary_dir})
    file(MAKE_DIRECTORY ${binary_dir}/libcaf_core)
    file(MAKE_DIRECTORY ${binary_dir}/libcaf_core/caf)
    file(MAKE_DIRECTORY ${binary_dir}/libcaf_core/caf/detail)
    file(MAKE_DIRECTORY ${binary_dir}/libcaf_io)
    file(MAKE_DIRECTORY ${binary_dir}/libcaf_openssl)
    file(MAKE_DIRECTORY ${binary_dir}/libcaf_test)

    # Define the include directories of the external project
    set(CAF_INCLUDE_DIRS ${binary_dir}/libcaf_core
                         ${binary_dir}/libcaf_io
		         ${binary_dir}/libcaf_openssl
		         ${binary_dir}/libcaf_test
		         ${binary_dir}
                         ${source_dir}/libcaf_core
                         ${source_dir}/libcaf_io
		         ${source_dir}/libcaf_openssl
		         ${source_dir}/libcaf_test
		         ${source_dir})
    # Define the libraries directory of the external project
    set(CAF_LIBRARIES ${binary_dir}/libcaf_core/libcaf_core.so.0.18.5
	              ${binary_dir}/libcaf_io/libcaf_io.so.0.18.5
		      ${binary_dir}/libcaf_openssl/libcaf_openssl.so.0.18.5)
    # create a target for CAF which will be imported into the global namespace 
    # to be usable by all other projects.
    add_library(ActorFramework INTERFACE IMPORTED GLOBAL)
    # set the include paths
    target_include_directories(ActorFramework INTERFACE ${CAF_INCLUDE_DIRS})
    # set the libraries
    target_link_libraries(ActorFramework INTERFACE ${CAF_LIBRARIES})
    # set the compiler flags
    target_compile_features(ActorFramework INTERFACE cxx_std_17)
    # C++ Actor Framework is ready to be used.
    message("-- Creating Target ActorFramework - done")
  endif()
endif()
