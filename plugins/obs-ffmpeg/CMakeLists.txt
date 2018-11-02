SET(MyCategory Plugins/ffmpeg)

set(obs-ffmpeg_HEADERS
	obs-ffmpeg-formats.h
	obs-ffmpeg-compat.h
	closest-pixel-format.h)

set(obs-ffmpeg_SOURCES
	obs-ffmpeg.c
	obs-ffmpeg-audio-encoders.c
	obs-ffmpeg-nvenc.c
	obs-ffmpeg-output.c
	obs-ffmpeg-mux.c
	obs-ffmpeg-source.c)

if(UNIX AND NOT APPLE)
	list(APPEND obs-ffmpeg_SOURCES
		obs-ffmpeg-vaapi.c)
	LIST(APPEND obs-ffmpeg_PLATFORM_DEPS
		${LIBVA_LBRARIES})
endif()


SET(LIB_EXTERNAL_DEPS
	FFMPEG_AVCODEC_LIBRARY 
	FFMPEG_AVDEVICE_LIBRARY
	FFMPEG_AVFILTER_LIBRARY
	FFMPEG_AVUTIL_LIBRARY  
	FFMPEG_AVFORMAT_LIBRARY
)
SET(LIB_SOURCES
	${obs-ffmpeg_HEADERS}
	${obs-ffmpeg_SOURCES}
)

SET(LIB_INTERNAL_DEPS
	libobs
	media-playback
	w32-pthreads
)

VSP_ADD_PLUGIN(obs-ffmpeg MODULE)

install_obs_plugin_with_data(obs-ffmpeg data)

add_subdirectory(ffmpeg-mux)
