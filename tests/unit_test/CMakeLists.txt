include(FetchContent)

set(CATCH_CONFIG_FAST_COMPILE ON CACHE BOOL "")
set(CATCH_CONFIG_NO_EXPERIMENTAL_STATIC_ANALYSIS_SUPPORT ON CACHE BOOL "")
FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.6.0
    GIT_SHALLOW ON)
FetchContent_MakeAvailable(Catch2)
include(Catch)

target_compile_features(Catch2 PRIVATE cxx_std_17)

set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
set_target_properties(Catch2 PROPERTIES COMPILE_OPTIONS "" EXPORT_COMPILE_COMMANDS OFF)
set_target_properties(Catch2WithMain PROPERTIES EXPORT_COMPILE_COMMANDS OFF)
get_target_property(CATCH2_INCLUDE_DIRS Catch2 INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(Catch2 SYSTEM INTERFACE ${CATCH2_INCLUDE_DIRS})

enable_testing()

file(GLOB_RECURSE TDL_TEST_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/tests/unit_test/*.cpp")
add_executable(TDLTest ${TDL_SOURCE} ${TDL_TEST_SOURCE})

target_link_libraries(TDLTest ${PNG_LIBRARIES} ${FREETYPE_LIBRARIES} ${OPENAL_LIBRARY} ${SNDFILE_LIBRARIES} ${X11_LIBRARIES} Catch2::Catch2WithMain )

add_test(NAME TDLTest COMMAND TDLTest)

option(COVERAGE "Build the demo" OFF)
if(COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")

    add_custom_target(coverage
        COMMAND mkdir -p coverage
        COMMAND ${CMAKE_MAKE_PROGRAM} test
        COMMAND lcov --directory . --capture --output-file coverage.info
        COMMAND lcov --remove coverage.info '/usr/*' --output-file coverage.info
        COMMAND lcov --list coverage.info
        COMMAND genhtml coverage.info --output-directory coverage
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()