SET(MyCategory deps)

add_subdirectory(w32-pthreads)

add_subdirectory(glad)
add_subdirectory(ipc-util)

if(BUILD_LIBFF)
	add_subdirectory(libff)
endif()

add_subdirectory(media-playback)
add_subdirectory(file-updater)
add_subdirectory(obs-scripting)

if(WIN32)
	add_subdirectory(blake2)
	add_subdirectory(lzma)
endif()

if(BUILD_CAPTIONS)
	add_subdirectory(libcaption)
endif()


add_subdirectory(jansson)

