project(test-input)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/libobs")

if(MSVC)
	set(test-input_PLATFORM_DEPS
		w32-pthreads)
endif()

set(test-input_SOURCES
	${test-input_PLATFORM_SOURCES}
	test-filter.c
	test-input.c
	test-sinewave.c
	sync-async-source.c
	sync-audio-buffering.c
	sync-pair-vid.c
	sync-pair-aud.c
	test-random.c)


SET(LIB_SOURCES
	${test-input_SOURCES}
)
SET(LIB_EXTERNAL_DEPS
	
)
SET(LIB_INTERNAL_DEPS
	libobs
)


SET(MyCategory Plugins)


VSP_ADD_PLUGIN(test-input MODULE)

install_obs_plugin_with_data(test-input data)
