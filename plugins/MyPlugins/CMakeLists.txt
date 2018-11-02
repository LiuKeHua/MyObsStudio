get_filename_component(MyProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)

SET(MyCategory Plugins/${MyProjectName})

return()

AddAllSubProjectForDirectory(${CMAKE_CURRENT_SOURCE_DIR})



