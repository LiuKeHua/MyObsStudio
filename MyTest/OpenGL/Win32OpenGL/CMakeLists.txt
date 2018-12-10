

SET(TARGET_SRC
	main.cpp
)


VSP_ADD_EXE(Win32OpenGL)
#set_target_properties(Win32OpenGL PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
#set_target_properties(Win32OpenGL PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
#set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")

target_link_libraries(Win32OpenGL
	opengl32
)

target_link_libraries(Win32OpenGL
	libobs)

install_obs_core(Win32OpenGL)

