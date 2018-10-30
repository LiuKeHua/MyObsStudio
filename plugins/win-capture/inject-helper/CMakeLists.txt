project(inject-helper)

set(inject-helper_HEADERS
	../inject-library.h
	../obfuscate.h)

set(inject-helper_SOURCES
	../inject-library.c
	../obfuscate.c
	inject-helper.c)

if(MSVC)
	add_compile_options($<IF:$<CONFIG:Debug>,/MTd,/MT>)
endif()

SET(TARGET_SRC
	${inject-helper_SOURCES}
)


if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	SET(outname inject-helper64)
ELSE()
	SET(outname inject-helper32)
ENDIF()


VSP_ADD_EXE(${outname})

install_obs_datatarget(${outname} "obs-plugins/win-capture")
