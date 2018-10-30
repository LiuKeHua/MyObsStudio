
set(ipc-util_HEADERS
	ipc-util/pipe.h)

set(ipc-util_HEADERS
	${ipc-util_HEADERS}
	ipc-util/pipe-windows.h)
set(ipc-util_SOURCES
		ipc-util/pipe-windows.c)

if(MSVC)
	add_compile_options($<IF:$<CONFIG:Debug>,/MTd,/MT> /Zl)
endif()


SET(LIB_SOURCES	
	${ipc-util_SOURCES}	
	${ipc-util_HEADERS}
)

VSP_ADD_LIBRARY(ipc-util STATIC)


target_include_directories(ipc-util
	PUBLIC .)
target_link_libraries(ipc-util)
