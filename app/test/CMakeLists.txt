cmake_minimum_required( VERSION 3.12 )

project( testit
	 VERSION 0.1.0
	 DESCRIPTION "A simple test")

add_executable(testit)

# the sources don't need to be exported to anywhere else
target_sources(testit PRIVATE "main.cpp")

# for buliding the executable it is necessary for the
# compiler to support cxx 17, however this is private
# to the implementation and anybody using the executable
# does not need to support cxx_std_17.
target_compile_features(testit PRIVATE cxx_std_17)

target_link_libraries(testit
	PRIVATE Qt::Core
	PRIVATE Qt::Widgets
	PRIVATE ActorFramework)
