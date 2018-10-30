
set(ffmpeg-mux_SOURCES
	ffmpeg-mux.c)

set(ffmpeg-mux_HEADERS
	ffmpeg-mux.h)



SET(TARGET_SRC
	${ffmpeg-mux_SOURCES}
	${ffmpeg-mux_HEADERS}
)

SET(LIB_INTERNAL_DEPS
	libobs
)
SET(LIB_EXTERNAL_DEPS
	FFMPEG_AVCODEC_LIBRARY 
	FFMPEG_AVDEVICE_LIBRARY
	FFMPEG_AVFILTER_LIBRARY
	FFMPEG_AVUTIL_LIBRARY  
	FFMPEG_AVFORMAT_LIBRARY
)


if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	SET(outname ffmpeg-mux64)
ELSE()
	SET(outname ffmpeg-mux32)
ENDIF()

VSP_ADD_EXE(${outname})

install_obs_datatarget(${outname} "obs-plugins/obs-ffmpeg")
