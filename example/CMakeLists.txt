
file(
        GLOB SRC_DIRS ${TDL_EXAMPLES_DIR}/*/
        LIST_DIRECTORIES true
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../include/TDL/Graphics/Widget/Default
        DESTINATION ${TDL_EXAMPLES_OUTPUT_DIR}/assets)

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
        add_executable(${dir_name} ${TDL_SOURCE} ${SRC_FILES})
        target_link_libraries(${dir_name} ${PNG_LIBRARIES} ${FREETYPE_LIBRARIES} ${SNDFILE_LIBRARIES} ${OPENAL_LIBRARY} nlohmann_json::nlohmann_json ${LIBEVDEV_LIBRARIES} ${LIBSIXEL_LIBRARIES})
        target_include_directories(${dir_name} PRIVATE ${SNDFILE_INCLUDE_DIRS} ${OPENAL_INCLUDE_DIR} ${LIBEVDEV_INCLUDE_DIRS} ${LIBSIXEL_LIBRARIES})
        message(STATUS "Adding example: ${dir_name}")
        set_target_properties(
                ${dir_name}
                PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${TDL_EXAMPLES_OUTPUT_DIR}
        )
    endif()
endforeach ()