project(win-test)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/libobs")

set(win-text_SOURCES
	test.cpp)



SET(MyCategory App)

SET(TARGET_SRC
	${win-text_SOURCES}
)

SET(APP_NAME win-test)

VSP_ADD_EXE(${APP_NAME})

set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
#set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")


target_link_libraries(${APP_NAME}
	libobs)
define_graphic_modules(${APP_NAME})

install_obs_core(${APP_NAME})
