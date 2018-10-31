

SET(TARGET_SRC
#	app.rc
	assets.h
	assets.cpp
	console.h
	console.cpp
	d3d.cpp
	d3d.h	
	d3d11.cpp
	d3d11.h	
	main.cpp
	platform.h
	renderer.cpp
	renderer9.cpp
	renderer11.cpp
	resource.h
	scene.h
	util.cpp
	util.h
)


get_filename_component(MyProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)



VSP_ADD_EXE(${MyProjectName})

set_target_properties(${MyProjectName} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_target_properties(${MyProjectName} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
#set_target_properties(${MyProjectName} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")


install_obs_core(${MyProjectName})

target_link_libraries (${MyProjectName} d3d9.lib d3d11.lib d2d1.lib dwrite.lib Shlwapi.lib)