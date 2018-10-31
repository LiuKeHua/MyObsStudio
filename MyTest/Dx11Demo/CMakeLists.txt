

SET(TARGET_SRC
	main.cpp
)


get_filename_component(MyProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)



VSP_ADD_EXE(${MyProjectName})

target_link_libraries (${MyProjectName} d3d9.lib d3d11.lib d2d1.lib dwrite.lib Shlwapi.lib d3dcompiler.lib)

install_obs_core(${MyProjectName})

set_target_properties(${MyProjectName} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_target_properties(${MyProjectName} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
#set_target_properties(${MyProjectName} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
