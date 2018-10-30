
include_directories(${OBS_JANSSON_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/libobs)
include_directories(${PROJECT_BINARY_DIR}/deps/jansson/include)


set(rtmp-services_SOURCES
	twitch.c
	rtmp-common.c
	rtmp-custom.c
	rtmp-services-main.c)

set(rtmp-services_HEADERS
	twitch.h
	rtmp-format-ver.h)

set(RTMP_SERVICES_URL
	"https://obsproject.com/obs2_update/rtmp-services"
	CACHE STRING "Default services package URL")
option(CHECK_FOR_SERVICE_UPDATES "Checks for service updates" OFF)

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/lookup-config.h.in"
	"${CMAKE_BINARY_DIR}/plugins/rtmp-services/lookup-config.h")

set(rtmp-services_config_HEADERS
	"${CMAKE_BINARY_DIR}/plugins/rtmp-services/lookup-config.h")




SET(LIB_SOURCES
	${rtmp-services_SOURCES}
	${rtmp-services_HEADERS}
	${rtmp-services_config_HEADERS}
)
SET(LIB_EXTERNAL_DEPS
	LIBx264_LIBRARY
	LIBCURL_LIBRARY
)
SET(LIB_INTERNAL_DEPS
	file-updater
	libobs
)


VSP_ADD_PLUGIN(rtmp-services MODULE)



target_include_directories(rtmp-services
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/plugins/rtmp-services>")

install_obs_plugin_with_data(rtmp-services data)
