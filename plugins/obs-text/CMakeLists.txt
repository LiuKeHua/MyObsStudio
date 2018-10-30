if (NOT WIN32)
	return()
endif()

project(obs-text)

if(WIN32)
	set(obs-text_PLATFORM_SOURCES
		gdiplus/obs-text.cpp)
	set(obs-text_PLATFORM_DEPS
		)
endif()


SET(LIB_SOURCES
	${obs-text_PLATFORM_SOURCES}
	${obs-text_PLATFORM_HEADERS}
)
SET(LIB_INTERNAL_DEPS
	libobs
)
VSP_ADD_PLUGIN(obs-text MODULE)

target_link_libraries(obs-text
	gdiplus
)
install_obs_plugin_with_data(obs-text data)
