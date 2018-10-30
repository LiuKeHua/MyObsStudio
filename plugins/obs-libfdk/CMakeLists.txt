project(obs-libfdk)

if(DISABLE_LIBFDK)
	message(STATUS "Libfdk support disabled")
	return()
endif()

find_package(Libfdk QUIET)
if(NOT LIBFDK_FOUND AND ENABLE_LIBFDK)
	message(FATAL_ERROR "Libfdk not found but set as enabled")
elseif(NOT LIBFDK_FOUND)
	message(STATUS "Libfdk not found - obs-libfdk plugin disabled")
	return()
endif()

include_directories(${LIBFDK_INCLUDE_DIRS})
add_definitions(${LIBFDK_DEFINITIONS})

set(obs-libfdk_SOURCES
	obs-libfdk.c)


SET(LIB_SOURCES
	${obs-libfdk_SOURCES}
)

SET(LIB_INTERNAL_DEPS
	libobs
)

VSP_ADD_PLUGIN(obs-libfdk MODULE)

install_obs_plugin_with_data(obs-libfdk data)
