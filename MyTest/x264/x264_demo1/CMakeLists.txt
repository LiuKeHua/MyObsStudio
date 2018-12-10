

SET(TARGET_SRC
	main.cpp
)


get_filename_component(MyProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

SET(LIB_EXTERNAL_DEPS
	LIBx264_LIBRARY
)

VSP_ADD_EXE(${MyProjectName})

target_link_libraries(${MyProjectName}
	libobs)

install_obs_core(${MyProjectName})

