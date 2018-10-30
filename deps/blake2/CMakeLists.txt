
add_compile_options($<IF:$<CONFIG:Debug>,/MTd,/MT> /Zl)

add_definitions(
	-Dinline=_inline
	-Drestrict=__restrict)

set(blake2_SOURCES
	src/blake2b-ref.c)

set(blake2_HEADERS
	src/blake2.h
	src/blake2-impl.h)


SET(LIB_SOURCES	
	${blake2_SOURCES}
	${blake2_HEADERS}
)

VSP_ADD_LIBRARY(blake2 STATIC)

