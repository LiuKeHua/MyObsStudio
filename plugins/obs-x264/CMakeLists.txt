

set(obs-x264_SOURCES
	obs-x264.c
	obs-x264-plugin-main.c)

SET(LIB_SOURCES
	${obs-x264_SOURCES}
)
SET(LIB_EXTERNAL_DEPS
	LIBx264_LIBRARY
)
SET(LIB_INTERNAL_DEPS
	libobs
)

VSP_ADD_PLUGIN(obs-x264 MODULE)


install_obs_plugin_with_data(obs-x264 data)
