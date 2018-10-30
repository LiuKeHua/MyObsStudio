
if(DISABLE_FREETYPE)
	message(STATUS "Freetype text plugin disabled")
	return()
endif()


set(text-freetype2_PLATFORM_SOURCES
		find-font.c
		find-font-windows.c)
include_directories(${FREETYPE_INCLUDE_DIRS})

set(text-freetype2_SOURCES
	find-font.h
	obs-convenience.c
	text-functionality.c
	text-freetype2.c
	obs-convenience.h
	text-freetype2.h)

SET(LIB_SOURCES
		${text-freetype2_PLATFORM_SOURCES}
	${text-freetype2_SOURCES}
)
SET(LIB_EXTERNAL_DEPS
	FREETYPE_LIBRARY
)
SET(LIB_INTERNAL_DEPS
	libobs
)

VSP_ADD_PLUGIN(text-freetype2 MODULE)

	

install_obs_plugin_with_data(text-freetype2 data)
