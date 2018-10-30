include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/libobs")

add_definitions(-DLIBOBS_EXPORTS)

set(obs-frontend-api_SOURCES
	obs-frontend-api.cpp)

set(obs-frontend-api_HEADERS
	obs-frontend-internal.hpp
	obs-frontend-api.h
)


SET(LIB_SOURCES	
	${obs-frontend-api_SOURCES}
	${obs-frontend-api_HEADERS}
)

SET(MyCategory Librarys)

VSP_ADD_LIBRARY(obs-frontend-api)

target_link_libraries(obs-frontend-api
	libobs
)

install_obs_core(obs-frontend-api)
