
SET(MyCategory MyTest)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/libobs")

#add_subdirectory(signal)
#add_subdirectory(Win32OpenGL)

AddAllSubProjectForDirectory(${CMAKE_CURRENT_SOURCE_DIR})


