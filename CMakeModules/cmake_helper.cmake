function(putils_get_subdirectories dst_list curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            list(APPEND dirlist ${child})
        endif()
	endforeach()
	set(${dst_list} ${dirlist} PARENT_SCOPE)
endfunction()

function(putils_get_runtime_output_directories dst_list)
	set(${dst_list}
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL}
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO}
		PARENT_SCOPE
	)
endfunction()

function(putils_src_files dst_list)
	set(allfiles "")
	foreach (dir ${ARGN})
		list(APPEND allfiles ${dir}/*.cpp ${dir}/*.hpp ${dir}/*.inl)
	endforeach()

	file(GLOB src_files ${allfiles})
	set(${dst_list} ${src_files} PARENT_SCOPE)
endfunction()
