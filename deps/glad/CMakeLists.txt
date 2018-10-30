find_package(OpenGL)

set(glad_SOURCES
	src/glad.c
	include/glad/glad.h)


set(glad_PLATFORM_SOURCES
	src/glad_wgl.c
	include/glad/glad_wgl.h)


set(glad_include_dirs
	PRIVATE ${OPENGL_INCLUDE_DIR})

SET(LIB_SOURCES	
	${glad_SOURCES}	
	${glad_PLATFORM_SOURCES} 
)

VSP_ADD_LIBRARY(glad)


target_compile_definitions(glad
	PRIVATE GLAD_GLAPI_EXPORT_BUILD)

target_include_directories(glad
	PUBLIC include
	${glad_include_dirs})

target_link_libraries(glad
	${glad_PLATFORM_DEPS}
	${OPENGL_gl_LIBRARY})

install_obs_core(glad)
