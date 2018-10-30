SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "add a postfix, usually empty on windows")




function(AddAllSubProjectForDirectory directory)
  file(GLOB  Directories  "${directory}/*")
  foreach(_var ${Directories})
    get_filename_component(MyProjectName ${_var} NAME)
    IF(NOT ( ${MyProjectName} MATCHES ".txt"))
#      message(${MyProjectName})
      ADD_subdirectory(${MyProjectName})
    ENDIF()
  endforeach()
endfunction()



##########���ñ�׼�����Ŀ¼�ṹ################################################################
SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin)
MAKE_DIRECTORY(${OUTPUT_BINDIR})
SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib)
MAKE_DIRECTORY(${OUTPUT_LIBDIR})

SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${OUTPUT_LIBDIR} CACHE PATH "build directory")
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${OUTPUT_BINDIR} CACHE PATH "build directory")
IF(MSVC_IDE)
  SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${OUTPUT_BINDIR} CACHE PATH "build directory")
ELSE()
  SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_LIBDIR} CACHE PATH "build directory")
ENDIF()


MACRO(CONFIG_OUT_FOLDER)
  # For each configuration (Debug, Release, MinSizeRel... and/or anything the user chooses)
  IF(CMAKE_CONFIGURATION_TYPES)
    FOREACH(CONF ${CMAKE_CONFIGURATION_TYPES})
      STRING(TOUPPER "${CONF}" CONF)
	  IF (${CONF} STREQUAL "DEBUG")  # ��Debug��������ֿ�
        SET(OUTPUT_FOLDER "MyDebug")
      ELSE()
	    SET(OUTPUT_FOLDER "Release")
      ENDIF (${CONF} STREQUAL "DEBUG")
     
      SET("CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONF}"  "${OUTPUT_LIBDIR}/${OUTPUT_FOLDER}/${CURRENT_ARCHIVE_PATH}")
      SET("CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONF}"  "${OUTPUT_BINDIR}/${OUTPUT_FOLDER}/${CURRENT_RUNTIME_PATH}")
      SET("CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF}"  "${OUTPUT_BINDIR}/${OUTPUT_FOLDER}/${CURRENT_LIBRARY_PATH}")

    ENDFOREACH()    
  ENDIF(CMAKE_CONFIGURATION_TYPES)
ENDMACRO(CONFIG_OUT_FOLDER )
##########################################################################




########���ù��̵�ǰ׺���ƺ͸ù������ڵķ���#####################################
MACRO(SET_PROJECTLABEL_AND_FOLDERGROUP FOLDERNAME)
  SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES FOLDER ${FOLDERNAME})

#�������ڹ����ļ�����ӱ�ǩ,���ɵĹ����ļ���ʽΪ:"${TARGET_LABEL}${TARGET_NAME}"
  SET(TARGET_LABEL "")
  IF( TARGET_LABEL )
    SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES PROJECT_LABEL "${TARGET_LABEL}${TARGET_NAME}")
  ENDIF(TARGET_LABEL)
ENDMACRO(SET_PROJECTLABEL_AND_FOLDERGROUP)
########################################################################
#------------------------------------------------------------------------------------------------------------#

#�����ǵ�ǰ���õ���DEBUG����RELEASE����Ӧ��DEBUG����RELEASE�Ŀ�
MACRO(LINK_WITH_VARIABLES TRGTNAME)
  FOREACH(varname ${ARGN})
    IF (WIN32)	
      IF(${varname}_RELEASE)
	    IF(${varname}_DEBUG)
          TARGET_LINK_LIBRARIES(${TRGTNAME} optimized "${${varname}_RELEASE}" debug "${${varname}_DEBUG}")
        ELSE(${varname}_DEBUG)
          TARGET_LINK_LIBRARIES(${TRGTNAME} optimized "${${varname}_RELEASE}" debug "${${varname}_RELEASE}" )
        ENDIF(${varname}_DEBUG)
      ELSE(${varname}_RELEASE)
        IF(${varname}_DEBUG)
          TARGET_LINK_LIBRARIES(${TRGTNAME} optimized "${${varname}}" debug "${${varname}_DEBUG}")
        ELSE(${varname}_DEBUG)
          TARGET_LINK_LIBRARIES(${TRGTNAME} optimized "${${varname}}" debug "${${varname}}" )
        ENDIF(${varname}_DEBUG)
      ENDIF(${varname}_RELEASE)
    ELSE (WIN32)
      IF(${varname}_RELEASE)
        TARGET_LINK_LIBRARIES(${TRGTNAME} "${${varname}_RELEASE}")
      ELSE(${varname}_RELEASE)
        TARGET_LINK_LIBRARIES(${TRGTNAME} "${${varname}}")
      ENDIF(${varname}_RELEASE)
    ENDIF (WIN32)
  ENDFOREACH(varname)
ENDMACRO(LINK_WITH_VARIABLES TRGTNAME)

########################################################################
#------------------------------------------------------------------------------------------------------------#

#��lib����صİ�����������LIB_EXTERNAL_DEPS(�ⲿ�����⣩��LIB_INTERNAL_DEPS���������ڲ��⣩
#��Դ�ļ���صİ���һ������TARGET_SRC �� TARGET_HEADER
#����ɾ�̬����߶�̬����صı���LIBRARY_TYPE
MACRO(VSP_ADD_EXE execName)
 
  SET(TARGET_NAME ${execName})

  CONFIG_OUT_FOLDER()

  ADD_EXECUTABLE(${TARGET_NAME} ${TARGET_SRC} ${TARGET_HEADER} ${VersionFilesOutputDllInfo})
  LINK_WITH_VARIABLES(${TARGET_NAME} ${LIB_EXTERNAL_DEPS})
  TARGET_LINK_LIBRARIES(${TARGET_NAME} ${LIB_INTERNAL_DEPS})

  SET_PROJECTLABEL_AND_FOLDERGROUP(${MyCategory})


  SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES PROJECT_LABEL "${TARGET_LABEL}")
  SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES OUTPUT_NAME "${TARGET_NAME}")
  SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES DEBUG_OUTPUT_NAME "${TARGET_NAME}${CMAKE_DEBUG_POSTFIX}")
  SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES RELEASE_OUTPUT_NAME "${TARGET_NAME}${CMAKE_RELEASE_POSTFIX}")
  SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES RELWITHDEBINFO_OUTPUT_NAME "${TARGET_NAME}${CMAKE_RELWITHDEBINFO_POSTFIX}")
  SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES MINSIZEREL_OUTPUT_NAME "${TARGET_NAME}${CMAKE_MINSIZEREL_POSTFIX}")
  
  if(WIN32)
    set_target_properties(${TGTNAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
    # set_target_properties(${TGTNAME} PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
    set_target_properties(${TGTNAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
  endif(WIN32)
  
  IF( BUILD_AT_UNICODE )
    ADD_DEFINITIONS(-DUNICODE)
  ENDIF( BUILD_AT_UNICODE )
ENDMACRO(VSP_ADD_EXE)

########################################################################
#------------------------------------------------------------------------------------------------------------#


#��lib����صİ�����������LIB_EXTERNAL_DEPS(�ⲿ�����⣩��LIB_INTERNAL_DEPS���������ڲ��⣩
#��Դ�ļ���صİ�����������LIB_PUBLIC_HEADERS��LIB_SOURCES
#����ɾ�̬����߶�̬����صı���LIBRARY_TYPE
MACRO(VSP_ADD_LIBRARY LIB_NAME )

  SET(TARGET_NAME ${LIB_NAME})
 
  SET(LIBRARY_TYPE SHARED)

  if(${ARGC} GREATER 1)
	 SET(LIBRARY_TYPE ${ARGV1})
  ENDIF()

  CONFIG_OUT_FOLDER()

  ADD_LIBRARY(${LIB_NAME} ${LIBRARY_TYPE}
    ${LIB_PUBLIC_HEADERS}
    ${LIB_SOURCES}
	${VersionFilesOutputDllInfo}
    )
  LINK_WITH_VARIABLES(${LIB_NAME} ${LIB_EXTERNAL_DEPS})
  TARGET_LINK_LIBRARIES(${LIB_NAME} ${LIB_INTERNAL_DEPS})
  

  SET_PROJECTLABEL_AND_FOLDERGROUP(${MyCategory})

  IF( BUILD_AT_UNICODE )
    ADD_DEFINITIONS(-DUNICODE)
  ENDIF( BUILD_AT_UNICODE )

ENDMACRO(VSP_ADD_LIBRARY)
###########################################################################
#��lib����صİ�����������LIB_EXTERNAL_DEPS(�ⲿ�����⣩��LIB_INTERNAL_DEPS���������ڲ��⣩
#��Դ�ļ���صİ�����������LIB_PUBLIC_HEADERS��LIB_SOURCES
#����ɾ�̬����߶�̬����صı���LIBRARY_TYPE
MACRO (VSP_ADD_PLUGIN LIB_NAME)
  SET(TARGET_NAME ${LIB_NAME})
  SET(LIBRARY_TYPE SHARED)

  if(${ARGC} GREATER 1)
	 SET(LIBRARY_TYPE ${ARGV1})
  ENDIF()

  CONFIG_OUT_FOLDER()
    
  # check unicode set
  IF( BUILD_AT_UNICODE )
    ADD_DEFINITIONS(-DUNICODE)
  ENDIF( BUILD_AT_UNICODE )
  
  ADD_LIBRARY(${TARGET_NAME} ${LIBRARY_TYPE}
    ${LIB_PUBLIC_HEADERS}
    ${LIB_SOURCES}
	${VersionFilesOutputDllInfo}
    )

  LINK_WITH_VARIABLES(${TARGET_NAME} ${LIB_EXTERNAL_DEPS})
  TARGET_LINK_LIBRARIES(${TARGET_NAME} ${LIB_INTERNAL_DEPS})
  LINK_WITH_VARIABLES(${TARGET_NAME} ${TARGET_COMMON_LIBRARIES})

  SET_PROJECTLABEL_AND_FOLDERGROUP(${MyCategory})

ENDMACRO(VSP_ADD_PLUGIN)
