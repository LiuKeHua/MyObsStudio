

SET(TARGET_SRC
#	dxgl.c
	main.cpp
)


get_filename_component(MyProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)


VSP_ADD_EXE(${MyProjectName})


#set_target_properties(${MyProjectName} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
#set_target_properties(${MyProjectName} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
#set_target_properties(${MyProjectName} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")


target_link_libraries(${MyProjectName}
	dxgi
	d3d11
	opengl32
)

install_obs_core(${MyProjectName})

