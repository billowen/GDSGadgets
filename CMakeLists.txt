cmake_minimum_required (VERSION 2.8.11)
add_definitions(-std=c++11)

project (GDSGadgets)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

option(BUILD_TEST "Whether to build the test exectable or not." OFF)

find_package(Qt5Gui)
FIND_PACKAGE(Qt5Widgets)


add_library(libGDSGadgets 
	boundingrect.cpp boundingrect.h)

target_link_libraries(libGDSGadgets Qt5::Gui)


if (BUILD_TEST)
	add_executable(testGDSGadgets main.cpp)
	TARGET_LINK_LIBRARIES(testGDSGadgets Qt5::Widgets Qt5::Gui)
else()
endif()
