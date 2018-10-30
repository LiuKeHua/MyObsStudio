SET(MyCategory Plugins/win-capture)

set(win-capture_HEADERS
	nt-stuff.h
	obfuscate.h
	app-helpers.h
	hook-helpers.h
	inject-library.h
	cursor-capture.h
	graphics-hook-info.h
	window-helpers.h
	dc-capture.h)

set(win-capture_SOURCES
	dc-capture.c
	obfuscate.c
	app-helpers.c
	inject-library.c
	cursor-capture.c
	game-capture.c
	window-helpers.c
	monitor-capture.c
	window-capture.c
	load-graphics-offsets.c
	duplicator-monitor-capture.c
	plugin-main.c)



SET(LIB_SOURCES
	${win-capture_SOURCES}
	${win-capture_HEADERS}
)
SET(LIB_EXTERNAL_DEPS
	psapi
)
SET(LIB_INTERNAL_DEPS
	libobs
	ipc-util
)

VSP_ADD_PLUGIN(win-capture MODULE)

install_obs_plugin_with_data(win-capture data)

add_subdirectory(graphics-hook)
add_subdirectory(get-graphics-offsets)
add_subdirectory(inject-helper)
