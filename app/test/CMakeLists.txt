cmake_minimum_required( VERSION 3.12 )

message("-- Creating Target testit: Try to find CAF ...")

ExternalProject_Get_Property(actorframework binary_dir)

# only use the specific installation of CAF in external/ folder
# disable all other search locations

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
  message("-- Creating Target testit: ... found it! ")
  project( testit
			VERSION 0.1.0
			DESCRIPTION "A simple test")

  find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

  qt5_wrap_ui(GROUP_CHAT_UI_HDR chatwindow.ui)
  qt5_wrap_cpp(GROUP_CHAT_MOC_SRC chatwidget.hpp)

  add_executable(testit
    qt_group_chat.cpp
    chatwidget.cpp
    ${GROUP_CHAT_MOC_SRC}
    ${GROUP_CHAT_UI_HDR} )

  target_compile_features(testit PRIVATE cxx_std_17)
  target_include_directories(testit PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_LIST_DIR} )

  target_link_libraries(testit
    PRIVATE Qt::Core
    PRIVATE Qt::Gui
    PRIVATE Qt::Widgets
    PRIVATE CAF::core
    PRIVATE CAF::io )
endif()