find_package(Threads REQUIRED)


set(HAVE_DBUS "0")
set(HAVE_PULSEAUDIO "0")
set(USE_XINPUT "0")

add_definitions(-DLIBOBS_EXPORTS)


set(libobs_image_loading_SOURCES
	graphics/graphics-ffmpeg.c)
set(libobs_image_loading_LIBRARIES
		${FFMPEG_AVCODEC_LIBRARIES})


if(WIN32)
	set(libobs_PLATFORM_SOURCES
		obs-win-crash-handler.c
		obs-windows.c
		util/threading-windows.c
		util/pipe-windows.c
		util/platform-windows.c)
	set(libobs_PLATFORM_HEADERS
		util/threading-windows.h
		util/windows/win-registry.h
		util/windows/win-version.h
		util/windows/ComPtr.hpp
		util/windows/CoTaskMemPtr.hpp
		util/windows/HRError.hpp
		util/windows/WinHandle.hpp)
	set(libobs_audio_monitoring_SOURCES
		audio-monitoring/win32/wasapi-output.c
		audio-monitoring/win32/wasapi-enum-devices.c
		)
	set(libobs_audio_monitoring_HEADERS
		audio-monitoring/win32/wasapi-output.h
		)
	set(libobs_PLATFORM_DEPS winmm psapi)
	if(MSVC)
		set(libobs_PLATFORM_DEPS
		${libobs_PLATFORM_DEPS}
		w32-pthreads)
	endif()
endif()

if(MSVC)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /EHc-")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc-")
endif()

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/obsconfig.h.in"
	"${CMAKE_BINARY_DIR}/config/obsconfig.h")

set(libobs_config_HEADERS
	"${CMAKE_BINARY_DIR}/config/obsconfig.h"
	obs-config.h)

set(libobs_callback_SOURCES
	callback/calldata.c
	callback/decl.c
	callback/signal.c
	callback/proc.c)
set(libobs_callback_HEADERS
	callback/calldata.h
	callback/decl.h
	callback/proc.h
	callback/signal.h)

set(libobs_graphics_SOURCES
	${libobs_image_loading_SOURCES}
	graphics/quat.c
	graphics/effect-parser.c
	graphics/axisang.c
	graphics/vec4.c
	graphics/vec2.c
	graphics/libnsgif/libnsgif.c
	graphics/texture-render.c
	graphics/image-file.c
	graphics/bounds.c
	graphics/matrix3.c
	graphics/matrix4.c
	graphics/vec3.c
	graphics/graphics.c
	graphics/shader-parser.c
	graphics/plane.c
	graphics/effect.c
	graphics/math-extra.c
	graphics/graphics-imports.c)
set(libobs_graphics_HEADERS
	graphics/plane.h
	graphics/quat.h
	graphics/input.h
	graphics/axisang.h
	graphics/shader-parser.h
	graphics/effect.h
	graphics/math-defs.h
	graphics/matrix4.h
	graphics/graphics.h
	graphics/graphics-internal.h
	graphics/libnsgif/libnsgif.h
	graphics/device-exports.h
	graphics/image-file.h
	graphics/vec2.h
	graphics/vec4.h
	graphics/matrix3.h
	graphics/vec3.h
	graphics/math-extra.h
	graphics/bounds.h
	graphics/effect-parser.h)

set(libobs_mediaio_SOURCES
	media-io/video-io.c
	media-io/video-fourcc.c
	media-io/video-matrices.c
	media-io/audio-io.c
	media-io/video-frame.c
	media-io/format-conversion.c
	media-io/audio-resampler-ffmpeg.c
	media-io/video-scaler-ffmpeg.c
	media-io/media-remux.c)
set(libobs_mediaio_HEADERS
	media-io/media-io-defs.h
	media-io/video-io.h
	media-io/audio-io.h
	media-io/audio-math.h
	media-io/video-frame.h
	media-io/format-conversion.h
	media-io/audio-resampler.h
	media-io/video-scaler.h
	media-io/media-remux.h
	media-io/frame-rate.h)

set(libobs_util_SOURCES
	util/array-serializer.c
	util/file-serializer.c
	util/base.c
	util/platform.c
	util/cf-lexer.c
	util/bmem.c
	util/config-file.c
	util/lexer.c
	util/dstr.c
	util/utf8.c
	util/crc32.c
	util/text-lookup.c
	util/cf-parser.c
	util/profiler.c)
set(libobs_util_HEADERS
	util/array-serializer.h
	util/file-serializer.h
	util/utf8.h
	util/crc32.h
	util/base.h
	util/text-lookup.h
	util/vc/vc_inttypes.h
	util/vc/vc_stdbool.h
	util/vc/vc_stdint.h
	util/bmem.h
	util/c99defs.h
	util/util_uint128.h
	util/cf-parser.h
	util/threading.h
	util/pipe.h
	util/cf-lexer.h
	util/darray.h
	util/circlebuf.h
	util/dstr.h
	util/serializer.h
	util/config-file.h
	util/lexer.h
	util/platform.h
	util/profiler.h
	util/profiler.hpp)

set(libobs_libobs_SOURCES
	${libobs_PLATFORM_SOURCES}
	obs-audio-controls.c
	obs-avc.c
	obs-encoder.c
	obs-service.c
	obs-source.c
	obs-source-deinterlace.c
	obs-source-transition.c
	obs-output.c
	obs-output-delay.c
	obs.c
	obs-properties.c
	obs-data.c
	obs-hotkey.c
	obs-hotkey-name-map.c
	obs-module.c
	obs-display.c
	obs-view.c
	obs-scene.c
	obs-audio.c
	obs-video.c)
set(libobs_libobs_HEADERS
	${libobs_PLATFORM_HEADERS}
	obs-audio-controls.h
	obs-defs.h
	obs-avc.h
	obs-encoder.h
	obs-service.h
	obs-internal.h
	obs.h
	obs-ui.h
	obs-properties.h
	obs-data.h
	obs-interaction.h
	obs-hotkey.h
	obs-hotkeys.h
	obs-module.h
	obs-scene.h
	obs-source.h
	obs-output.h
	obs-ffmpeg-compat.h
	obs.hpp)

set(libobs_SOURCES
	${libobs_callback_SOURCES}
	${libobs_graphics_SOURCES}
	${libobs_mediaio_SOURCES}
	${libobs_util_SOURCES}
	${libobs_libobs_SOURCES}
	${libobs_audio_monitoring_SOURCES}
	)

set(libobs_HEADERS
	${libobs_config_HEADERS}
	${libobs_callback_HEADERS}
	${libobs_graphics_HEADERS}
	${libobs_mediaio_HEADERS}
	${libobs_util_HEADERS}
	${libobs_libobs_HEADERS}
	${libobs_audio_monitoring_HEADERS}
	)

source_group("callback\\Source Files" FILES ${libobs_callback_SOURCES})
source_group("callback\\Header Files" FILES ${libobs_callback_HEADERS})
source_group("graphics\\Source Files" FILES ${libobs_graphics_SOURCES})
source_group("graphics\\Header Files" FILES ${libobs_graphics_HEADERS})
source_group("libobs\\Source Files" FILES ${libobs_libobs_SOURCES})
source_group("libobs\\Header Files" FILES ${libobs_libobs_HEADERS})
source_group("media-io\\Source Files" FILES ${libobs_mediaio_SOURCES})
source_group("media-io\\Header Files" FILES ${libobs_mediaio_HEADERS})
source_group("util\\Source Files" FILES ${libobs_util_SOURCES})
source_group("util\\Header Files" FILES ${libobs_util_HEADERS})
source_group("audio-monitoring\\Source Files" FILES ${libobs_audio_monitoring_SOURCES})
source_group("audio-monitoring\\Header Files" FILES ${libobs_audio_monitoring_HEADERS})

if(BUILD_CAPTIONS)
	include_directories(${CMAKE_SOURCE_DIR}/deps/libcaption)
	set(libobs_PLATFORM_DEPS
		${libobs_PLATFORM_DEPS}
		caption)
endif()


SET(LIB_SOURCES
	${libobs_SOURCES} 
	${libobs_HEADERS} 
)

SET(LIB_EXTERNAL_DEPS
	FFMPEG_AVCODEC_LIBRARY 
	FFMPEG_AVDEVICE_LIBRARY
	FFMPEG_AVFILTER_LIBRARY
	FFMPEG_AVUTIL_LIBRARY  
	FFMPEG_AVFORMAT_LIBRARY
	FFMPEG_SWSCALE_LIBRARY
	FFMPEG_SWRESAMPLE_LIBRARY
	ZLIB_LIBRARY
	
)
INCLUDE_DIRECTORIES(
	${PROJECT_BINARY_DIR}/deps/jansson/include
)

SET(LIB_INTERNAL_DEPS
	jansson
	w32-pthreads
)

VSP_ADD_LIBRARY(libobs)



target_link_libraries(libobs
	${libobs_PLATFORM_DEPS}
)


set_target_properties(libobs PROPERTIES
	OUTPUT_NAME libobs
	VERSION "0"
	SOVERSION "0")
target_compile_definitions(libobs
	PUBLIC
		HAVE_OBSCONFIG_H)


target_compile_options(libobs
	PUBLIC
		"${THREADS_DEFINITIONS}")
target_include_directories(libobs
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
		"$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/config>"
		"$<INSTALL_INTERFACE:${OBS_INCLUDE_DESTINATION}>")

install_obs_core(libobs EXPORT LibObs)
install_obs_data(libobs data libobs)
install_obs_headers(${libobs_HEADERS})

obs_install_additional(libobs)
