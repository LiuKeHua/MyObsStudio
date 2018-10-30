
set(obs-transitions_SOURCES
	obs-transitions.c
	transition-slide.c
	transition-swipe.c
	transition-fade.c
	transition-cut.c
	transition-fade-to-color.c
	transition-luma-wipe.c
	transition-stinger.c
	)



SET(LIB_SOURCES
	${obs-transitions_SOURCES}
)
SET(LIB_INTERNAL_DEPS
	libobs
)
VSP_ADD_PLUGIN(obs-transitions MODULE)


install_obs_plugin_with_data(obs-transitions data)
