
find_package(TDL REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../lib/cmake/TDL)

file(
        GLOB SRC_DIRS ${TDL_EXAMPLES_DIR}/*/
        LIST_DIRECTORIES true
)

add_custom_command(
        OUTPUT ${TDL_EXAMPLES_OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TDL_EXAMPLES_OUTPUT_DIR}
        COMMENT "Creating demo directory"
)

add_custom_target(create_demo_dir ALL DEPENDS ${TDL_EXAMPLES_OUTPUT_DIR})

foreach(dir ${SRC_DIRS})

    if(NOT IS_DIRECTORY ${dir})
        continue()
    endif()

    get_filename_component(dir_name ${dir} NAME_WE)

    file(
            GLOB_RECURSE SRC_FILES ${dir}/*.cpp
            LIST_DIRECTORIES false
            EXCLUDE ${dir}/assets/*
            EXCLUDE ${dir}/build/*
            EXCLUDE ${dir}/cmake-build-debug/*
    )

    if(SRC_FILES)

        add_executable(${dir_name} ${SRC_FILES})

        target_link_libraries(${dir_name}
                TDL::TDL
        )

        message(STATUS "Adding example: ${dir_name}")
        set_target_properties(
                ${dir_name}
                PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${TDL_EXAMPLES_OUTPUT_DIR}
        )
        add_dependencies(${dir_name} create_demo_dir)

    endif()

endforeach ()