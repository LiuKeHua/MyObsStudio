
include_directories(${PROJECT_SOURCE_DIR}/libobs)

if(WIN32 AND NOT MINGW)
	include_directories(../w32-pthreads)
	set(file-updater_PLATFORM_DEPS
		w32-pthreads)
endif()

set(file-updater_HEADERS
	file-updater/file-updater.h)
set(file-updater_SOURCES
	file-updater/file-updater.c)

SET(LIB_SOURCES	
	${file-updater_SOURCES}
	${file-updater_HEADERS}
)

VSP_ADD_LIBRARY(file-updater STATIC)


target_include_directories(file-updater
	PUBLIC .)

