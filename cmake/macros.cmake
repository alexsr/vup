macro(ADD_PROJECTS project_list source_dir bin_dir)
    file(GLOB_RECURSE file_paths RELATIVE ${source_dir} ${source_dir}/*/CMakeLists.txt)
    set(dir_list "")
    set(project_list "")
    foreach(file_path ${file_paths})
        get_filename_component(dir_path ${file_path} DIRECTORY)
        set(dir_list ${dir_list} ${dir_path})
        get_filename_component(project_name ${dir_path} NAME)
        string(REPLACE " " "_" project_name ${project_name})
        set(${project_list} ${${project_list}} ${project_name})
    endforeach()
    list(REMOVE_DUPLICATES dir_list)
    list(REMOVE_DUPLICATES project_list)
    foreach(dir ${dir_list})
        message("Adding ${source_dir}/${dir}")
        add_subdirectory(${source_dir}/${dir} ${bin_dir}/${dir})
    endforeach()
endmacro()

macro(COPY_DLL name from to)
    file(GLOB dll_name ${from}/${name}*.dll)
    if (dll_name STREQUAL "")
        message("ERROR: ASSIMP DLL COULD NOT BE COPIED")
    else ()
        get_filename_component(dll_name ${dll_name} NAME)
        message("COPY TO ${to}/${dll_name}")
        file(COPY ${from}/${dll_name} DESTINATION ${to})
    endif ()
endmacro()
