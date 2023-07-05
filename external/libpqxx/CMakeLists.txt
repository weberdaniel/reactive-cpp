cmake_minimum_required(VERSION 3.14)

project(external.libpqxx)

set(VERSION 0.0.0)
set(DESCRIPTION "libpqxx")

include(ExternalProject)

# add all local cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

message("-- Creating Target libpqxx")

message("-- Creating Target libpqxx: Downloading ... ")
ExternalProject_Add(libpqxx URL ${CMAKE_CURRENT_LIST_DIR}/libpqxx
  CONFIGURE_COMMAND <SOURCE_DIR>/configure --build=x86_64 CXXFLAGS=-std=c++17
                    --host=x86_64-pc-linux --target=x86_64-pc-linux
  )

message("-- Creating Target libpqxx - done")
