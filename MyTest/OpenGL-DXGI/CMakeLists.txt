get_filename_component(MyProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

SET(MyCategory MyTest/${MyProjectName})

AddAllSubProjectForDirectory(${CMAKE_CURRENT_SOURCE_DIR})



