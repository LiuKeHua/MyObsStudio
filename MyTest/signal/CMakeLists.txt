

SET(TARGET_SRC
	main.cpp
)


VSP_ADD_EXE(signal)

target_link_libraries(signal
	libobs)

install_obs_core(signal)

