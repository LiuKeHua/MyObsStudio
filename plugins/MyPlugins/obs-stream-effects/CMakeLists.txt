
# Automatic Versioning
Set(VERSION_MAJOR 0)
Set(VERSION_MINOR 4)
Set(VERSION_PATCH 2)
Set(VERSION_TWEAK 0)
SET(PROJECT_VERSION_TWEAK 0)
Set(PROJECT_COMMIT "N/A")

# Configure Version Header
Configure_File(
	"${CMAKE_CURRENT_SOURCE_DIR}/source/version.h.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/version.h"
)


# All Warnings, Extra Warnings, Pedantic
If(MSVC)
	# Force to always compile with W4
	If(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		String(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	Else()
		Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	EndIf()
ElseIf(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	# Update If necessary
	Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
EndIf()

math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")


SET(PROJECT_PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/source/plugin.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/plugin.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/filter-displacement.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/filter-displacement.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/filter-blur.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/filter-blur.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/filter-shape.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/filter-shape.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/filter-transform.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/filter-transform.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/filter-custom-shader.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/filter-custom-shader.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/source-mirror.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/source-mirror.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gfx-effect-source.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gfx-effect-source.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gfx-source-texture.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gfx-source-texture.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-helper.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-helper.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-effect.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-effect.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-indexbuffer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-indexbuffer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-limits.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-mipmapper.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-mipmapper.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-rendertarget.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-rendertarget.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-sampler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-sampler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-texture.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-texture.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-vertex.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-vertex.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-vertexbuffer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/gs-vertexbuffer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/obs-audio-capture.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/obs-audio-capture.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/version.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/strings.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/utility.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/utility.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/util-math.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/util-math.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/util-memory.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/util-memory.cpp"
)

################################################################################
# Target
################################################################################

SET(LIB_SOURCES
	${PROJECT_PRIVATE}
)

SET(LIB_INTERNAL_DEPS
	libobs
)

SET(PROJECT_NAME obs-stream-effect)

VSP_ADD_PLUGIN(obs-stream-effect MODULE)


install_obs_plugin_with_data(obs-stream-effect data)

# Definitions
If (WIN32)
	Target_Compile_Definitions(${PROJECT_NAME}
		PRIVATE
			_CRT_SECURE_NO_WARNINGS
			# windows.h
			WIN32_LEAN_AND_MEAN
			NOGPICAPMASKS
			NOVIRTUALKEYCODES
			NOWINMESSAGES
			NOWINSTYLES
			NOSYSMETRICS
			NOMENUS
			NOICONS
			NOKEYSTATES
			NOSYSCOMMANDS
			NORASTEROPS
			NOSHOWWINDOW
			NOATOM
			NOCLIPBOARD
			NOCOLOR
			NOCTLMGR
			NODRAWTEXT
			NOGDI
			NOKERNEL
			#NOUSER
			NONLS
			NOMB
			NOMEMMGR
			NOMETAFILE
			NOMINMAX
			#NOMSG
			NOOPENFILE
			NOSCROLL
			NOSERVICE
			NOSOUND
			NOTEXTMETRIC
			NOWH
			NOWINOFFSETS
			NOCOMM
			NOKANJI
			NOHELP
			NOPROFILER
			NODEFERWINDOWPOS
			NOMCX
			NOIME
			NOMDI
			NOINOUT
	)
EndIf()
