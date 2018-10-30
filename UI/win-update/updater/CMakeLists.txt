
if(DISABLE_UPDATE_MODULE)
	return()
endif()


include_directories(${PROJECT_SOURCE_DIR}/deps/jansson/src)
include_directories(${PROJECT_SOURCE_DIR}/deps/lzma/liblzma/api)
include_directories(${PROJECT_SOURCE_DIR}/deps/blake2/src)


set(updater_HEADERS
	../win-update-helpers.hpp
	resource.h
	updater.hpp
	)
set(updater_SOURCES
	../win-update-helpers.cpp
	updater.cpp
	patch.cpp
	http.cpp
	hash.cpp
	updater.rc
	)

add_definitions(-DNOMINMAX -DUNICODE -D_UNICODE)
if(MSVC)
	add_compile_options($<IF:$<CONFIG:Debug>,/MTd,/MT>)
endif()


SET(TARGET_SRC
	${updater_HEADERS}
	${updater_SOURCES}
)

SET(LIB_EXTERNAL_DEPS

	Qt5Core_LIBRARIES
	Qt5Widgets_LIBRARIES
	Qt5Network_LIBRARIES
	Qt5Svg_LIBRARIES

	ZLIB_LIBRARY
)

SET(LIB_INTERNAL_DEPS
	lzma
	blake2
	jansson
)

VSP_ADD_EXE(updater)

SET(APP_NAME updater)

set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
#set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")

target_link_libraries(updater
	psapi
	comctl32
	shell32
	winhttp
	msvcrtd
)

install_obs_core(${APP_NAME})

