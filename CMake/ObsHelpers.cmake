set(OBS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/Myout")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(_lib_suffix 64)
else()
	set(_lib_suffix 32)
endif()

if(WIN32 OR APPLE)
	set(_struct_def FALSE)
else()
	set(_struct_def TRUE)
endif()

option(INSTALLER_RUN "Build a multiarch installer, needs to run indenepdently after both archs have compiled" FALSE)
option(UNIX_STRUCTURE "Build with standard unix filesystem structure" ${_struct_def})


if(NOT UNIX_STRUCTURE)
	set(OBS_DATA_DESTINATION "data")

	set(OBS_EXECUTABLE_DESTINATION "bin/${_lib_suffix}bit")
	set(OBS_EXECUTABLE32_DESTINATION "bin/32bit")
	set(OBS_EXECUTABLE64_DESTINATION "bin/64bit")
	set(OBS_LIBRARY_DESTINATION "bin/${_lib_suffix}bit")
	set(OBS_LIBRARY32_DESTINATION "bin/32bit")
	set(OBS_LIBRARY64_DESTINATION "bin/64bit")
	set(OBS_PLUGIN_DESTINATION "obs-plugins/${_lib_suffix}bit")
	set(OBS_PLUGIN32_DESTINATION "obs-plugins/32bit")
	set(OBS_PLUGIN64_DESTINATION "obs-plugins/64bit")

	set(OBS_DATA_PATH "../../${OBS_DATA_DESTINATION}")
	set(OBS_INSTALL_PREFIX "")
	set(OBS_RELATIVE_PREFIX "../../")

	set(OBS_SCRIPT_PLUGIN_DESTINATION "${OBS_DATA_DESTINATION}/obs-scripting/${_lib_suffix}bit")

	set(OBS_CMAKE_DESTINATION "cmake")
	set(OBS_INCLUDE_DESTINATION "include")
	set(OBS_UNIX_STRUCTURE "0")

	set(OBS_SCRIPT_PLUGIN_PATH "${OBS_RELATIVE_PREFIX}${OBS_SCRIPT_PLUGIN_DESTINATION}")

endif()

function(obs_finish_bundle)
	if(NOT APPLE OR UNIX_STRUCTURE)
		return()
	endif()

	install(CODE
		"if(DEFINED ENV{FIXUP_BUNDLE})
			execute_process(COMMAND \"${CMAKE_SOURCE_DIR}/cmake/osxbundle/fixup_bundle.sh\" . bin WORKING_DIRECTORY \"\${CMAKE_INSTALL_PREFIX}\")
		endif()")
endfunction()


function(obs_helper_copy_dir target target_configs source dest)
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND "${CMAKE_COMMAND}"
			"-DCONFIG=$<CONFIGURATION>"
			"-DTARGET_CONFIGS=${target_configs}"
			"-DINPUT=${source}"
			"-DOUTPUT=${dest}"
			-P "${CMAKE_SOURCE_DIR}/cmake/copy_helper.cmake"
		VERBATIM)
endfunction()

function(obs_install_additional maintarget)
	set(addfdir "${CMAKE_SOURCE_DIR}/additional_install_files")

	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(_lib_suffix 64)
	else()
		set(_lib_suffix 32)
	endif()

	obs_helper_copy_dir(${maintarget} ALL
		"${addfdir}/misc/"
		"${OBS_OUTPUT_DIR}/$<CONFIGURATION>/")
	obs_helper_copy_dir(${maintarget} ALL
		"${addfdir}/data/"
		"${OBS_OUTPUT_DIR}/$<CONFIGURATION>/${OBS_DATA_DESTINATION}/")
	obs_helper_copy_dir(${maintarget} ALL
		"${addfdir}/libs${_lib_suffix}/"
		"${OBS_OUTPUT_DIR}/$<CONFIGURATION>/${OBS_LIBRARY_DESTINATION}/")
	obs_helper_copy_dir(${maintarget} ALL
		"${addfdir}/exec${_lib_suffix}/"
		"${OBS_OUTPUT_DIR}/$<CONFIGURATION>/${OBS_EXECUTABLE_DESTINATION}/")
	obs_helper_copy_dir(${maintarget} "Release;MinSizeRel;RelWithDebInfo"
		"${addfdir}/exec${_lib_suffix}r/"
		"${OBS_OUTPUT_DIR}/$<CONFIGURATION>/${OBS_EXECUTABLE_DESTINATION}/")
	obs_helper_copy_dir(${maintarget} "Debug"
		"${addfdir}/exec${_lib_suffix}d/"
		"${OBS_OUTPUT_DIR}/$<CONFIGURATION>/${OBS_EXECUTABLE_DESTINATION}/")
	obs_helper_copy_dir(${maintarget} "Release;MinSizeRel;RelWithDebInfo"
		"${addfdir}/libs${_lib_suffix}r/"
		"${OBS_OUTPUT_DIR}/$<CONFIGURATION>/${OBS_LIBRARY_DESTINATION}/")
	obs_helper_copy_dir(${maintarget} "Debug"
		"${addfdir}/libs${_lib_suffix}d/"
		"${OBS_OUTPUT_DIR}/$<CONFIGURATION>/${OBS_LIBRARY_DESTINATION}/")
endfunction()


function(install_obs_headers)
	foreach(hdr ${ARGN})
		if(IS_ABSOLUTE "${hdr}")
			set(subdir)
		else()
			get_filename_component(subdir "${hdr}" DIRECTORY)
			if(subdir)
				set(subdir "/${subdir}")
			endif()
		endif()
		install(FILES "${hdr}" DESTINATION "${OBS_INCLUDE_DESTINATION}${subdir}")
	endforeach()
endfunction()

function(obs_debug_copy_helper target dest)
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND "${CMAKE_COMMAND}"
			"-DCONFIG=$<CONFIGURATION>"
			"-DFNAME=$<TARGET_FILE_NAME:${target}>"
			"-DINPUT=$<TARGET_FILE_DIR:${target}>"
			"-DOUTPUT=${dest}"
			-P "${CMAKE_SOURCE_DIR}/cmake/copy_on_debug_helper.cmake"
		VERBATIM)
endfunction()

function(install_obs_pdb ttype target)
	if(NOT MSVC)
		return()
	endif()

	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(_bit_suffix "64bit")
	else()
		set(_bit_suffix "32bit")
	endif()

	obs_debug_copy_helper(${target} "${CMAKE_CURRENT_BINARY_DIR}/pdbs")
	if("${ttype}" STREQUAL "PLUGIN")
		obs_debug_copy_helper(${target} "${OBS_OUTPUT_DIR}/$<CONFIGURATION>/obs-plugins/${_bit_suffix}")
	elseif("${ttype}" STREQUAL "CORE")
		obs_debug_copy_helper(${target} "${OBS_OUTPUT_DIR}/$<CONFIGURATION>/bin/${_bit_suffix}")
	endif()
endfunction()

function(install_obs_core target)
	if(APPLE)
		set(_bit_suffix "")
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(_bit_suffix "64bit/")
	else()
		set(_bit_suffix "32bit/")
	endif()

	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy
			"$<TARGET_FILE:${target}>"
			"${OBS_OUTPUT_DIR}/$<CONFIGURATION>/bin/${_bit_suffix}$<TARGET_FILE_NAME:${target}>"
		VERBATIM)

	install_obs_pdb(CORE ${target})
endfunction()

function(install_obs_bin target mode)
	foreach(bin ${ARGN})
		if(APPLE)
			set(_bit_suffix "")
		elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(_bit_suffix "64bit/")
		else()
			set(_bit_suffix "32bit/")
		endif()

		if(NOT IS_ABSOLUTE "${bin}")
			set(bin "${CMAKE_CURRENT_SOURCE_DIR}/${bin}")
		endif()

		get_filename_component(fname "${bin}" NAME)

		if(NOT "${mode}" MATCHES "INSTALL_ONLY")
			add_custom_command(TARGET ${target} POST_BUILD
				COMMAND "${CMAKE_COMMAND}" -E copy
					"${bin}"
					"${OBS_OUTPUT_DIR}/$<CONFIGURATION>/bin/${_bit_suffix}${fname}"
				VERBATIM)
		endif()
	endforeach()
endfunction()

function(install_obs_plugin target)
	if(APPLE)
		set(_bit_suffix "")
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(_bit_suffix "64bit/")
	else()
		set(_bit_suffix "32bit/")
	endif()

	set_target_properties(${target} PROPERTIES
		PREFIX "")
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy
			"$<TARGET_FILE:${target}>"
			"${OBS_OUTPUT_DIR}/$<CONFIGURATION>/obs-plugins/${_bit_suffix}$<TARGET_FILE_NAME:${target}>"
		VERBATIM)
	install_obs_pdb(PLUGIN ${target})
endfunction()

function(install_obs_data target datadir datadest)
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_directory
			"${CMAKE_CURRENT_SOURCE_DIR}/${datadir}" "${OBS_OUTPUT_DIR}/$<CONFIGURATION>/data/${datadest}"
		VERBATIM)
endfunction()

function(install_obs_data_file target datafile datadest)
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E make_directory
			"${OBS_OUTPUT_DIR}/$<CONFIGURATION>/data/${datadest}"
		VERBATIM)
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy
			"${CMAKE_CURRENT_SOURCE_DIR}/${datafile}" "${OBS_OUTPUT_DIR}/$<CONFIGURATION>/data/${datadest}"
		VERBATIM)
endfunction()

function(install_obs_datatarget target datadest)
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy
			"$<TARGET_FILE:${target}>"
			"${OBS_OUTPUT_DIR}/$<CONFIGURATION>/data/${datadest}/$<TARGET_FILE_NAME:${target}>"
		VERBATIM)
endfunction()

function(install_obs_plugin_with_data target datadir)
	install_obs_plugin(${target})
	install_obs_data(${target} "${datadir}" "obs-plugins/${target}")
endfunction()

function(define_graphic_modules target)
	foreach(dl_lib opengl d3d9 d3d11)
		string(TOUPPER ${dl_lib} dl_lib_upper)
		if(TARGET libobs-${dl_lib})
			if(UNIX AND UNIX_STRUCTURE)
				target_compile_definitions(${target}
					PRIVATE
					DL_${dl_lib_upper}="$<TARGET_SONAME_FILE_NAME:libobs-${dl_lib}>"
					)
			else()
				target_compile_definitions(${target}
					PRIVATE
					DL_${dl_lib_upper}="$<TARGET_FILE_NAME:libobs-${dl_lib}>"
					)
			endif()
		else()
			target_compile_definitions(${target}
				PRIVATE
				DL_${dl_lib_upper}=""
				)
		endif()
	endforeach()
endfunction()
