
include_directories(
	${CMAKE_SOURCE_DIR}/libobs
	${FFMPEG_INCLUDE_DIRS}
	)

set(media-playback_HEADERS
	media-playback/decode.h
	media-playback/media.h
	)
set(media-playback_SOURCES
	media-playback/decode.c
	media-playback/media.c
	)


SET(LIB_SOURCES	
	${media-playback_HEADERS}
	${media-playback_SOURCES}
)

VSP_ADD_LIBRARY(media-playback STATIC)

target_include_directories(media-playback
	PUBLIC .
	)

