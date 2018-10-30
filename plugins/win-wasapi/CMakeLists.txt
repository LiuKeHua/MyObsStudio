project(win-wasapi)

set(win-wasapi_HEADERS
	enum-wasapi.hpp)

set(win-wasapi_SOURCES
	win-wasapi.cpp
	enum-wasapi.cpp
	plugin-main.cpp)


SET(LIB_SOURCES
	${win-wasapi_SOURCES}
	${win-wasapi_HEADERS}
)
SET(LIB_EXTERNAL_DEPS
	
)
SET(LIB_INTERNAL_DEPS
	libobs
)

VSP_ADD_PLUGIN(win-wasapi MODULE)

install_obs_plugin_with_data(win-wasapi data)
