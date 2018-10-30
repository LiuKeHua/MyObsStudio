project(get-graphics-offsets)

set(get-graphics-offsets_HEADERS
	../graphics-hook-info.h
	../hook-helpers.h
	get-graphics-offsets.h)

set(get-graphics-offsets_SOURCES
	get-graphics-offsets.c
	dxgi-offsets.cpp
	d3d8-offsets.cpp
	d3d9-offsets.cpp)

if(MSVC)
	add_compile_options($<IF:$<CONFIG:Debug>,/MTd,/MT>)
endif()




SET(TARGET_SRC
	${get-graphics-offsets_SOURCES}
	${get-graphics-offsets_HEADERS}
)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	SET(outname get-graphics-offsets64)
ELSE()
	SET(outname get-graphics-offsets32)
ENDIF()


VSP_ADD_EXE(${outname})

target_link_libraries(${outname}
	d3d9.lib
	dxgi.lib
	d3d11.lib)

install_obs_datatarget(${outname} "obs-plugins/win-capture")
