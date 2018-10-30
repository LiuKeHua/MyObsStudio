project(graphics-hook)

set(COMPILE_D3D12_HOOK FALSE CACHE BOOL "Compile D3D12 hook support (required windows 10 SDK)")

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/graphics-hook-config.h.in"
	"${CMAKE_BINARY_DIR}/plugins/win-capture/graphics-hook/config/graphics-hook-config.h")


set(graphics-hook_HEADERS
	"${CMAKE_BINARY_DIR}/plugins/win-capture/graphics-hook/config/graphics-hook-config.h"
	graphics-hook.h
	../graphics-hook-info.h
	../hook-helpers.h
	../funchook.h
	../obfuscate.h
	gl-decs.h
	d3d9-patches.hpp)

set(graphics-hook_SOURCES
	graphics-hook.c
	../funchook.c
	../obfuscate.c
	gl-capture.c
	d3d8-capture.cpp
	d3d9-capture.cpp
	dxgi-capture.cpp
	d3d10-capture.cpp
	d3d11-capture.cpp
	d3d12-capture.cpp)

if(MSVC)
	add_compile_options("$<IF:$<CONFIG:Debug>,/MTd,/MT>")
endif()


SET(LIB_SOURCES
	${graphics-hook_SOURCES}
	${graphics-hook_HEADERS}
)
SET(LIB_EXTERNAL_DEPS
	psapi
)
SET(LIB_INTERNAL_DEPS
	libobs
	ipc-util
)


if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	SET(outname graphics-hook64)
ELSE()
	SET(outname graphics-hook32)
ENDIF()

VSP_ADD_LIBRARY(${outname})


target_include_directories(${outname} PUBLIC
	"${CMAKE_BINARY_DIR}/plugins/win-capture/graphics-hook/config")

install_obs_datatarget(${outname} "obs-plugins/win-capture")
