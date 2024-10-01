cmake_minimum_required(VERSION 3.10)

project(
        TDL
        VERSION 0.1
        DESCRIPTION "A Terminal Drawing Library"
        LANGUAGES CXX
)

option(BUILD_DEMO "Build the demo" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_TESTS "Build tests" OFF)

find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)
find_package(OpenAL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SNDFILE REQUIRED sndfile)
file(GLOB_RECURSE TDL_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${PNG_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TDL_EXAMPLES_DIR            ${CMAKE_CURRENT_SOURCE_DIR}/example)
set(TDL_EXAMPLES_OUTPUT_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/demo)
set(TDL_LIB_DIR                 ${CMAKE_CURRENT_SOURCE_DIR}/lib)

if (BUILD_TESTS)
    include(${CMAKE_CURRENT_SOURCE_DIR}/tests/unit_test/CMakeLists.txt)
endif()

if (BUILD_DEMO)
    include(${TDL_EXAMPLES_DIR}/CMakeLists.txt)
endif()
message("Building shared libraries: ${BUILD_SHARED_LIBS}")
find_package(Doxygen)

if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

if (BUILD_SHARED_LIBS)
    add_library(TDL SHARED ${TDL_SOURCE})

    target_link_libraries(TDL ${PNG_LIBRARIES} ${FREETYPE_LIBRARIES} ${OPENAL_LIBRARY} ${SNDFILE_LIBRARIES} ${GPM_LIBRARIES})

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/tdl
            DESTINATION include
            FILES_MATCHING PATTERN "*.hpp")

    install(TARGETS TDL
            EXPORT TDLTargets
            LIBRARY DESTINATION lib
            PUBLIC_HEADER DESTINATION include)
endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/TDLConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/TDLConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/TDLConfig.cmake"
        INSTALL_DESTINATION lib/cmake/TDL
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/TDLConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/TDLConfigVersion.cmake"
        DESTINATION lib/cmake/TDL
)

install(EXPORT TDLTargets
        FILE TDLTargets.cmake
        NAMESPACE TDL::
        DESTINATION lib/cmake/TDL
)