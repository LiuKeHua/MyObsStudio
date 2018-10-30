project(image-source)

if(MSVC)
	set(image-source_PLATFORM_DEPS
		w32-pthreads)
endif()

set(image-source_SOURCES
	image-source.c
	color-source.c
	obs-slideshow.c)


SET(LIB_SOURCES
	${image-source_SOURCES}
)

SET(LIB_INTERNAL_DEPS
	libobs
)

VSP_ADD_PLUGIN(image-source MODULE)


install_obs_plugin_with_data(image-source data)
