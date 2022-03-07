cmake_minimum_required( VERSION 3.12 )

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
	# ${Qt6Core_INCLUDE_DIRS}
	# ${Qt6Gui_INCLUDE_DIRS}
	# ${Qt6Widgets_INCLUDE_DIRS})
target_link_libraries(testit
	PRIVATE Qt::Core
	PRIVATE Qt::Gui
	PRIVATE Qt::Widgets
	PRIVATE ActorFramework)
