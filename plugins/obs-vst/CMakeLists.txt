if(DISABLE_UI)
	message(STATUS "UI disabled,so vst plugin disabled")
	return()
endif()



set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(Qt5Widgets REQUIRED)

if(APPLE)
	find_package(Qt5MacExtras REQUIRED)
endif(APPLE)

option(VST_USE_BUNDLED_HEADERS "Build with Bundled Headers" ON)

if(VST_USE_BUNDLED_HEADERS)
	message(STATUS "Using the bundled VST header.")
	include_directories(vst_header)
	set(vst_HEADER
		vst_header/aeffectx.h)
else()
	set(VST_INCLUDE_DIR "" CACHE PATH
		"Path to Steinburg headers (e.g. C:/VST3 SDK/pluginterfaces/vst2.x)")

	message(WARNING "You should only use the Steinburg headers for debugging or local
	 builds. It is illegal to distribute the Steinburg headers with anything, and
	 possibly against the GPL to distribute the binaries from the resultant compile.")
	include_directories(${VST_INCLUDE_DIR})
	set(vst_HEADER
		${VST_INCLUDE_DIR}/aeffectx.h)
endif()

if(APPLE)
	find_library(FOUNDATION_FRAMEWORK Foundation)
	find_library(COCOA_FRAMEWORK Cocoa)
endif(APPLE)

set(obs-vst_SOURCES
	obs-vst.cpp
	VSTPlugin.cpp
	EditorWidget.cpp)

if(APPLE)
	list(APPEND obs-vst_SOURCES
		mac/VSTPlugin-osx.mm
		mac/EditorWidget-osx.mm)

elseif(WIN32)
	list(APPEND obs-vst_SOURCES
		win/VSTPlugin-win.cpp
		win/EditorWidget-win.cpp)

elseif("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
	list (APPEND obs-vst_SOURCES
		linux/VSTPlugin-linux.cpp
		linux/EditorWidget-linux.cpp)
endif()

list(APPEND obs-vst_HEADERS
	headers/vst-plugin-callbacks.hpp
	headers/EditorWidget.h
	headers/VSTPlugin.h)

SET(LIB_SOURCES
	${obs-vst_SOURCES}
	${obs-vst_HEADERS}
	${vst-HEADER}
)
SET(LIB_EXTERNAL_DEPS
	Qt5Core_LIBRARIES
	Qt5Widgets_LIBRARIES
)
SET(LIB_INTERNAL_DEPS
	libobs
)

VSP_ADD_PLUGIN(obs-vst MODULE)



install_obs_plugin_with_data(obs-vst data)
