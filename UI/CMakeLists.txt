
SET(MyCategory App)



find_package(Qt5Core)
find_package(Qt5Network)
find_package(Qt5Widgets)

INCLUDE_DIRECTORIES(
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Network_INCLUDE_DIRS}
	${Qt5Svg_INCLUDE_DIRS}
	${Qt5Sql_INCLUDE_DIRS}
)

add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Quick_DEFINITIONS})
add_definitions(${Qt5Concurrent_DEFINITIONS})



if(DISABLE_UI)
	message(STATUS "UI disabled")
	return()
elseif(ENABLE_UI)
	set(FIND_MODE REQUIRED)
else()
	set(FIND_MODE QUIET)
endif()

if(BROWSER_AVAILABLE_INTERNAL)
	add_definitions(-DBROWSER_AVAILABLE)
endif()

add_subdirectory(obs-frontend-api)

# ----------------------------------------------------------------------------

set(DISABLE_UPDATE_MODULE FALSE CACHE BOOL "Disables building the update module")

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)


if(NOT Qt5Widgets_FOUND)
	if (ENABLE_UI)
		message(FATAL_ERROR "Failed to find Qt5")
	else()
		message(STATUS "Qt5 not found - UI disabled")
		return()
	endif()
endif()



include_directories(${FFMPEG_INCLUDE_DIRS})
include_directories(SYSTEM "obs-frontend-api")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/libobs")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/deps/libff")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/deps/json11")
if(BROWSER_AVAILABLE_INTERNAL)
	include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/plugins/obs-browser/panel")
endif()


if(WIN32)
	include_directories(${OBS_JANSSON_INCLUDE_DIRS})
	include_directories(${BLAKE2_INCLUDE_DIR})

	set(obs_PLATFORM_SOURCES
		platform-windows.cpp
		win-update/update-window.cpp
		win-update/win-update.cpp
		win-update/win-update-helpers.cpp
		obs.rc)
	set(obs_PLATFORM_HEADERS
		win-update/update-window.hpp
		win-update/win-update.hpp
		win-update/win-update-helpers.hpp)
	set(obs_PLATFORM_LIBRARIES
		crypt32
		blake2
		${OBS_JANSSON_IMPORT})
endif()

set(obs_libffutil_SOURCES
	../deps/libff/libff/ff-util.c
	)
set(obs_libffutil_HEADERS
	../deps/libff/libff/ff-util.h
	)

if(MSVC)
	set_source_files_properties(
		../deps/libff/libff/ff-util.c
		PROPERTIES COMPILE_FLAGS -Dinline=__inline
		)
endif()

set(obs_SOURCES
	${obs_PLATFORM_SOURCES}
	${obs_libffutil_SOURCES}
	../deps/json11/json11.cpp
	obs-app.cpp
	api-interface.cpp
	window-basic-main.cpp
	window-basic-stats.cpp
	window-basic-filters.cpp
	window-basic-settings.cpp
	window-basic-interaction.cpp
	window-basic-properties.cpp
	window-basic-auto-config.cpp
	window-basic-main-outputs.cpp
	window-basic-source-select.cpp
	window-basic-auto-config-test.cpp
	window-basic-main-scene-collections.cpp
	window-basic-main-transitions.cpp
	window-basic-main-dropfiles.cpp
	window-basic-main-profiles.cpp
	window-basic-status-bar.cpp
	window-basic-adv-audio.cpp
	window-basic-transform.cpp
	window-basic-preview.cpp
	window-basic-about.cpp
	window-namedialog.cpp
	window-log-reply.cpp
	window-projector.cpp
	window-remux.cpp
	source-tree.cpp
	properties-view.cpp
	focus-list.cpp
	menu-button.cpp
	double-slider.cpp
	volume-control.cpp
	adv-audio-control.cpp
	item-widget-helpers.cpp
	visibility-checkbox.cpp
	locked-checkbox.cpp
	horizontal-scroll-area.cpp
	vertical-scroll-area.cpp
	visibility-item-widget.cpp
	slider-absoluteset-style.cpp
	source-list-widget.cpp
	qt-display.cpp
	crash-report.cpp
	hotkey-edit.cpp
	source-label.cpp
	remote-text.cpp
	audio-encoders.cpp
	qt-wrappers.cpp)

set(obs_HEADERS
	${obs_PLATFORM_HEADERS}
	${obs_libffutil_HEADERS}
	../deps/json11/json11.hpp
	obs-app.hpp
	platform.hpp
	window-main.hpp
	window-basic-main.hpp
	window-basic-stats.hpp
	window-basic-filters.hpp
	window-basic-settings.hpp
	window-basic-interaction.hpp
	window-basic-properties.hpp
	window-basic-auto-config.hpp
	window-basic-main-outputs.hpp
	window-basic-source-select.hpp
	window-basic-about.hpp
	window-basic-status-bar.hpp
	window-basic-adv-audio.hpp
	window-basic-transform.hpp
	window-basic-preview.hpp
	window-namedialog.hpp
	window-log-reply.hpp
	window-projector.hpp
	window-remux.hpp
	source-tree.hpp
	properties-view.hpp
	properties-view.moc.hpp
	display-helpers.hpp
	balance-slider.hpp
	double-slider.hpp
	focus-list.hpp
	menu-button.hpp
	mute-checkbox.hpp
	volume-control.hpp
	adv-audio-control.hpp
	item-widget-helpers.hpp
	visibility-checkbox.hpp
	locked-checkbox.hpp
	horizontal-scroll-area.hpp
	expand-checkbox.hpp
	vertical-scroll-area.hpp
	visibility-item-widget.hpp
	slider-absoluteset-style.hpp
	source-list-widget.hpp
	qt-display.hpp
	crash-report.hpp
	hotkey-edit.hpp
	source-label.hpp
	remote-text.hpp
	audio-encoders.hpp
	qt-wrappers.hpp
	clickable-label.hpp)

set(obs_UI
	forms/NameDialog.ui
	forms/AutoConfigStartPage.ui
	forms/AutoConfigVideoPage.ui
	forms/AutoConfigStreamPage.ui
	forms/AutoConfigTestPage.ui
	forms/ColorSelect.ui
	forms/OBSLogReply.ui
	forms/OBSBasic.ui
	forms/OBSBasicTransform.ui
	forms/OBSBasicFilters.ui
	forms/OBSBasicSettings.ui
	forms/OBSBasicSourceSelect.ui
	forms/OBSBasicInteraction.ui
	forms/OBSUpdate.ui
	forms/OBSRemux.ui
	forms/OBSAbout.ui)

source_group("Ui" FILES  ${obs_UI})

set(obs_QRC
	forms/obs.qrc)

qt5_wrap_ui(obs_UI_HEADERS ${obs_UI})
qt5_add_resources(obs_QRC_SOURCES ${obs_QRC})


SET(TARGET_SRC
	${obs_SOURCES}
	${obs_HEADERS}
	${obs_UI_HEADERS}
	${obs_QRC_SOURCES}
)

INCLUDE_DIRECTORIES(
	${PROJECT_BINARY_DIR}/deps/jansson/include
	${PROJECT_SOURCE_DIR}/deps/blake2/src
)


SET(LIB_EXTERNAL_DEPS
	FFMPEG_AVCODEC_LIBRARY 
	FFMPEG_AVDEVICE_LIBRARY
	FFMPEG_AVFILTER_LIBRARY
	FFMPEG_AVUTIL_LIBRARY  

	Qt5Core_LIBRARIES
	Qt5Widgets_LIBRARIES
	Qt5Network_LIBRARIES
	Qt5Svg_LIBRARIES

	LIBCURL_LIBRARY
)

SET(LIB_INTERNAL_DEPS
	libobs
	blake2
	obs-frontend-api
)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	SET(OBS_NAME obs64)
ELSE()
	SET(OBS_NAME obs32)
ENDIF()

VSP_ADD_EXE(${OBS_NAME})

target_link_libraries(${OBS_NAME}
	crypt32
)


define_graphic_modules(${OBS_NAME})

install_obs_core(${OBS_NAME})
install_obs_data(${OBS_NAME} data obs-studio)
#install_obs_data_file(${OBS_NAME} ../AUTHORS obs-studio/authors)


add_subdirectory(frontend-plugins)
add_subdirectory(win-update/updater)
