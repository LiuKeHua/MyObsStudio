project(win-decklink)

if(DISABLE_DECKLINK)
	message(STATUS "decklink plugin disabled")
	return()
endif()

include(IDLFileHelper)

set(win-decklink-sdk_IDLS
	decklink-sdk/DeckLinkAPI.idl
	)

set(win-decklink-sdk_HEADERS
	decklink-sdk/DeckLinkAPIVersion.h
	)

set(win-decklink_HEADERS
	../decklink-devices.hpp
	../DecklinkOutput.hpp
	../const.h
	../platform.hpp
	../DecklinkInput.hpp
	../DecklinkBase.h
	../decklink-device-instance.hpp
	../decklink-device-discovery.hpp
	../decklink-device.hpp
	../decklink-device-mode.hpp
	../audio-repack.h
	../audio-repack.hpp
	)

set(win-decklink_SOURCES
	../plugin-main.cpp
	../decklink-devices.cpp
	../DecklinkOutput.cpp
	../decklink-source.cpp
	../decklink-output.cpp
	../DecklinkInput.cpp
	../DecklinkBase.cpp
	../decklink-device-instance.cpp
	../decklink-device-discovery.cpp
	../decklink-device.cpp
	../decklink-device-mode.cpp
	../audio-repack.c
	platform.cpp
	)

add_idl_files(win-decklink-sdk_GENERATED_FILES
	${win-decklink-sdk_IDLS}
	)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
)


SET(LIB_SOURCES
	${win-decklink_SOURCES}
	${win-decklink_HEADERS}
	${win-decklink-sdk_HEADERS}
	${win-decklink-sdk_GENERATED_FILES}
)
SET(LIB_EXTERNAL_DEPS
	
)
SET(LIB_INTERNAL_DEPS
	libobs
)

VSP_ADD_PLUGIN(win-decklink MODULE)

install_obs_plugin_with_data(win-decklink ../data)
