cmake_minimum_required( VERSION 3.12 )

project( svdemo
        VERSION 0.1.0
        DESCRIPTION "A simple test")

add_executable(svdemo
        main.cpp
        supervisor.h worker.cpp worker.h messages.h types.h supervisor.cpp)

target_compile_features(svdemo PRIVATE cxx_std_17)
target_include_directories(svdemo PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_LIST_DIR} )

target_link_libraries(svdemo
        PRIVATE ActorFramework)
