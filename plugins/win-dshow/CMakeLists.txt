if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libdshowcapture/dshowcapture.hpp")
	message(STATUS "libdshowcapture submodule not found!  Please fetch submodules.  win-dshow plugin disabled.")
	return()
endif()

project(win-dshow)


set(win-dshow_HEADERS
	encode-dstr.hpp
	ffmpeg-decode.h)

set(win-dshow_SOURCES
	win-dshow.cpp
	win-dshow-encoder.cpp
	dshow-plugin.cpp
	ffmpeg-decode.c)

set(libdshowcapture_SOURCES
	libdshowcapture/source/capture-filter.cpp
	libdshowcapture/source/output-filter.cpp
	libdshowcapture/source/dshowcapture.cpp
	libdshowcapture/source/dshowencode.cpp
	libdshowcapture/source/device.cpp
	libdshowcapture/source/encoder.cpp
	libdshowcapture/source/dshow-base.cpp
	libdshowcapture/source/dshow-demux.cpp
	libdshowcapture/source/dshow-enum.cpp
	libdshowcapture/source/dshow-formats.cpp
	libdshowcapture/source/dshow-media-type.cpp
	libdshowcapture/source/dshow-encoded-device.cpp
	libdshowcapture/source/log.cpp)

set(libdshowcapture_HEADERS
	libdshowcapture/dshowcapture.hpp
	libdshowcapture/source/IVideoCaptureFilter.h
	libdshowcapture/source/capture-filter.hpp
	libdshowcapture/source/output-filter.hpp
	libdshowcapture/source/device.hpp
	libdshowcapture/source/encoder.hpp
	libdshowcapture/source/dshow-base.hpp
	libdshowcapture/source/dshow-demux.hpp
	libdshowcapture/source/dshow-device-defs.hpp
	libdshowcapture/source/dshow-enum.hpp
	libdshowcapture/source/dshow-formats.hpp
	libdshowcapture/source/dshow-media-type.hpp
	libdshowcapture/source/log.hpp)


SET(LIB_SOURCES
	${win-dshow_SOURCES}
	${win-dshow_HEADERS}
	${libdshowcapture_SOURCES}
	${libdshowcapture_HEADERS}
)
SET(LIB_EXTERNAL_DEPS
	strmiids
	ksuser
	wmcodecdspuuid
)
SET(LIB_INTERNAL_DEPS
	libobs
)
SET(LIB_EXTERNAL_DEPS
	FFMPEG_AVCODEC_LIBRARY 
	FFMPEG_AVUTIL_LIBRARY  
)

VSP_ADD_PLUGIN(win-dshow MODULE)

target_link_libraries(win-dshow
	strmiids
	ksuser
	wmcodecdspuuid
)
install_obs_plugin_with_data(win-dshow data)
