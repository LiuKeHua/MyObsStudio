cmake_minimum_required(VERSION 3.2.0)
project(MyOBS_Studio VERSION 1.18.3)


SET(CMAKE_DEBUG_POSTFIX "" CACHE STRING "add a postfix, usually d on window")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows" )
SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "add a postfix, usually empty on windows")

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "possible configurations" FORCE)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/Common;${CMAKE_MODULE_PATH}")
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/version_utils;${CMAKE_MODULE_PATH}")



#消除QT的AUTOMOC 和 AUTOUIC警告
#参加 https://cmake.org/cmake/help/v3.10/policy/CMP0071.html
if(POLICY CMP0071)
  cmake_policy(SET CMP0071 OLD)
endif()


if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	SET(BUILD_X64 true)
ELSE()
	SET(BUILD_X64 false)
ENDIF()


include(generate_product_version)
generate_product_version(
   VersionFilesOutputDllInfo
   NAME               "Equalizer Parallel Rending"
   BUNDLE             "并行渲染library"
   ICON               ""
   VERSION_MAJOR       2
   VERSION_MINOR       0
   VERSION_PATCH       1
   VERSION_REVISION    3
   COMPANY_NAME        "liukehua880123@163.com"
   COMPANY_COPYRIGHT   "xxx, Ltd. (C) Copyright 2018"
   COMMENTS            "Parallel Rending v${VERSION_MAJOR}.${VERSION_MINOR}"
   ORIGINAL_FILENAME   ${NAME}
   INTERNAL_NAME       ${NAME} 
   FILE_DESCRIPTION    ${NAME}
)


INCLUDE(UtilityMacros)
INCLUDE(ObsHelpers)



FIND_PACKAGE(ThirdParty)

INCLUDE_DIRECTORIES(	
	${THIRDPARTY_INCLUDE_DIR}
	${PROJECT_SOURCE_DIR}/deps/jansson/src
)

set(ENABLE_SCRIPTING False  CACHE BOOL "Enables scripting")
set(SCRIPTING_ENABLED OFF CACHE BOOL "Interal global cmake variable" FORCE)


SET(MyCategory Librarys)

option(BUILD_CAPTIONS "Build captions" FALSE)

set(OBS_RELEASE_CANDIDATE_MAJOR 0)
set(OBS_RELEASE_CANDIDATE_MINOR 0)
set(OBS_RELEASE_CANDIDATE_PATCH 0)
set(OBS_RELEASE_CANDIDATE 0)


if(WIN32)
	add_definitions(-DUNICODE -D_UNICODE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS)
endif()
if(MSVC)
	set(CMAKE_C_FLAGS_DEBUG "/DDEBUG=1 /D_DEBUG=1 ${CMAKE_C_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_DEBUG "/DDEBUG=1 /D_DEBUG=1 ${CMAKE_C_FLAGS_DEBUG}")

	if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
		set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
	endif()
ENDIF()


#Librarys
add_subdirectory(deps)
add_subdirectory(libobs)
add_subdirectory(libobs-opengl)
add_subdirectory(libobs-d3d11)

#App
add_subdirectory(UI)

#Plugins

add_subdirectory(plugins)


add_subdirectory(test)
add_subdirectory(MyTest)

include(CopyMSVCBins)