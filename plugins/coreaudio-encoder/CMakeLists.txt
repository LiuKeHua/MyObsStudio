project(coreaudio-encoder)

set(coreaudio-encoder_SOURCES
	encoder.cpp)

if (WIN32)
	set(coreaudio-encoder_HEADERS windows-imports.h)
	set(coreaudio-encoder_LIBS )

	if (MINGW)
		set_source_files_properties(${coreaudio-encoder_SOURCES}
			PROPERTIES COMPILE_FLAGS "-Wno-multichar")
	endif()
else()
	find_library(COREFOUNDATION CoreFoundation)
	find_library(COREAUDIO CoreAudio)
	find_library(AUDIOTOOLBOX AudioToolbox)
	set(coreaudio-encoder_HEADERS )
	set(coreaudio-encoder_LIBS
		${COREFOUNDATION}
		${COREAUDIO}
		${AUDIOTOOLBOX})

	include_directories(${COREFOUNDATION}
		${COREAUDIO}
		${AUDIOTOOLBOX})
endif()



SET(LIB_SOURCES
		${coreaudio-encoder_SOURCES}
	${coreaudio-encoder_HEADERS}
)
SET(LIB_EXTERNAL_DEPS
	LIBx264_LIBRARY
)
SET(LIB_INTERNAL_DEPS
	libobs
)

VSP_ADD_PLUGIN(coreaudio-encoder MODULE)

install_obs_plugin_with_data(coreaudio-encoder data)
