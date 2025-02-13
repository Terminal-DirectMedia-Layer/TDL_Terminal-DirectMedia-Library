cmake_minimum_required(VERSION 3.10)

project(
        TDL_DEMO
        VERSION 0.1
        DESCRIPTION "The demo project for TDL"
        LANGUAGES CXX
)

# Set the CMAKE_PREFIX_PATH to include /usr/local

find_package(TDL REQUIRED)

message(STATUS "Current working directory: ${CMAKE_CURRENT_SOURCE_DIR}")

############################################## START SETTER ##############################################
set(TDL_EXAMPLES_DIR            ${CMAKE_CURRENT_SOURCE_DIR}/../example)
set(TDL_EXAMPLES_OUTPUT_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/../demo)
set(TDL_EXAMPLES_BUILD_DIR      ${TDL_EXAMPLES_DIR}/build)
############################################### END SETTER ###############################################

file(
        GLOB SRC_DIRS ${TDL_EXAMPLES_DIR}/*/
        LIST_DIRECTORIES true
        EXCLUDE ${dir}/assets/*
        EXCLUDE ${dir}/build/*
        EXCLUDE ${dir}/cmake-build-debug/*
        EXCLUDE ${dir}/build
)

add_custom_command(
        OUTPUT ${TDL_EXAMPLES_BUILD_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${TDL_EXAMPLES_BUILD_DIR}
        COMMENT "Creating build directory"
)

add_custom_target(create_build_dir ALL DEPENDS ${TDL_EXAMPLES_BUILD_DIR})

add_custom_command(
        OUTPUT ${TDL_EXAMPLES_BUILD_DIR}/CMakeCache.txt
        COMMAND ${CMAKE_COMMAND} -S ${TDL_EXAMPLES_DIR} -B ${TDL_EXAMPLES_BUILD_DIR}
        COMMENT "Configuring example project"
        DEPENDS create_build_dir
)

add_custom_target(configure_examples
        DEPENDS ${TDL_EXAMPLES_BUILD_DIR}/CMakeCache.txt
)

add_custom_target(build_examples
        COMMAND ${CMAKE_COMMAND} --build ${TDL_EXAMPLES_BUILD_DIR}
        WORKING_DIRECTORY ${TDL_EXAMPLES_BUILD_DIR}
        COMMENT "Building examples"
        DEPENDS configure_examples
)

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
            EXCLUDE ${dir}/build
    )

    if(SRC_FILES)
        add_executable(${dir_name} ${SRC_FILES})
        target_include_directories(${dir_name} PRIVATE ${TDL_INCLUDE_DIR})
        target_link_libraries(${dir_name} PRIVATE TDL::TDL)

        message(STATUS "Adding example: ${dir_name}")
        set_target_properties(
                ${dir_name}
                PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${TDL_EXAMPLES_OUTPUT_DIR}
        )
    endif()
endforeach()