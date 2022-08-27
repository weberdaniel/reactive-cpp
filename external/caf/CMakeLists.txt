cmake_minimum_required(VERSION 3.14)

project(external.caf)

set(VERSION 0.18.5)
set(DESCRIPTION "CAF")

include(ExternalProject)

# add all local cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

message("-- Creating Target ActorFramework")
if(NOT DOCKER) 
  message("-- Creating Target ActorFramework: Downloading... docker is NOT used!")
  ExternalProject_Add(actorframework URL ${CMAKE_CURRENT_LIST_DIR}/actor-framework
	CONFIGURE_COMMAND <SOURCE_DIR>/configure --build-type=Debug --log-level=TRACE --disable-examples --disable-tools --enable-utility-targets --enable-runtime-checks --enable-shared-libs 
	--build-dir=<BINARY_DIR>
  INSTALL_COMMAND cmake -E echo "-- Skip install step.")
endif()

message("-- Creating Target ActorFramework - done")
