cmake_minimum_required(VERSION 3.10)

project(
        TDL
        VERSION 0.1
        DESCRIPTION "A Terminal Drawing Library"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -pg")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g3 -pg")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -pg -fsanitize=address")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3 -pg -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g3 -pg -fsanitize=address")

option(BUILD_DEMO "Build the demo" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_TESTS "Build tests" OFF)

find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)
find_package(OpenAL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSIXEL REQUIRED libsixel)
pkg_check_modules(SNDFILE REQUIRED sndfile)
pkg_check_modules(LIBEVDEV REQUIRED libevdev)

file(GLOB_RECURSE TDL_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/tracy/public/tracy ${PNG_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS} ${LIBEVDEV_INCLUDE_DIRS} ${LIBSIXEL_INCLUDE_DIRS})

set(TDL_EXAMPLES_DIR            ${CMAKE_CURRENT_SOURCE_DIR}/example)
set(TDL_EXAMPLES_OUTPUT_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/demo)
set(TDL_LIB_DIR                 ${CMAKE_CURRENT_SOURCE_DIR}/lib)

if (BUILD_TESTS)
    include(${CMAKE_CURRENT_SOURCE_DIR}/tests/unit_test/CMakeLists.txt)
endif()

#if (BUILD_DEMO)
add_subdirectory(${TDL_EXAMPLES_DIR})
#endif()
message("Building shared libraries: ${BUILD_SHARED_LIBS}")

if (BUILD_SHARED_LIBS)
    add_library(TDL SHARED ${TDL_SOURCE})

    target_link_libraries(TDL ${PNG_LIBRARIES} ${FREETYPE_LIBRARIES} ${OPENAL_LIBRARY} ${SNDFILE_LIBRARIES} ${GPM_LIBRARIES} nlohmann_json::nlohmann_json ${LIBEVDEV_LIBRARIES} ${LIBSIXEL_LIBRARIES})  # Add ${LIBEVDEV_LIBRARIES}

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/tdl
            DESTINATION include
            FILES_MATCHING PATTERN "*.hpp")

    install(TARGETS TDL
            EXPORT TDLTargets
            LIBRARY DESTINATION lib
            PUBLIC_HEADER DESTINATION include)

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
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/include/TDL/Graphics/Widget/Default
        DESTINATION ${CMAKE_BINARY_DIR}/assets)