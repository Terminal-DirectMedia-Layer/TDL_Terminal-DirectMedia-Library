cmake_minimum_required(VERSION 3.10)

project(
        TDL
        VERSION 0.1
        DESCRIPTION "A Terminal Drawing Library"
        LANGUAGES CXX
)

include(CMakePackageConfigHelpers)

########################################### START CMAKE FLAGS ###########################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
########################################### END CMAKE FLAGS #############################################

########################################### START DEBUG FLAGS ###########################################
set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEBUG} -g3 -pg -fsanitize=address")
set(CMAKE_C_FLAGS_DEBUG             "${CMAKE_C_FLAGS_DEBUG} -g3 -pg -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG    "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g3 -pg -fsanitize=address")
############################################ END DEBUG FLAGS ############################################

########################################### START SOURCE PATH ##########################################
set(TDL_EXAMPLES_DIR            ${CMAKE_CURRENT_SOURCE_DIR}/example)
set(TDL_EXAMPLES_OUTPUT_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/demo)
set(TDL_LIB_DIR                 ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(TDL_INCLUDE_DIR             ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TDL_SOURCE_DIR              ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(TDL_TEST_DIR                ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit_test)

set(TDL_SIXEL_DIR               ${TDL_SOURCE_DIR}/TDL/Graphics/Display/Strategy/Sixel)
set(TDL_TTY_DIR                 ${TDL_SOURCE_DIR}/TDL/Graphics/Display/Strategy/TTY)
set(TDL_ASCII_DIR               ${TDL_SOURCE_DIR}/TDL/Graphics/Display/Strategy/Ascii)

set(TDL_SIXEL_INCL_DIR           ${TDL_INCLUDE_DIR}/TDL/Graphics/Display/Strategy/Sixel)
set(TDL_TTY_INCL_DIR             ${TDL_INCLUDE_DIR}/TDL/Graphics/Display/Strategy/TTY)
set(TDL_ASCII_INCL_DIR           ${TDL_INCLUDE_DIR}/TDL/Graphics/Display/Strategy/Ascii)

set(CMAKE_EXTERNAL_DIR           ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
########################################### END SOURCE PATH ############################################

########################################### START CUSTOM TARGETS ########################################
add_custom_target(install_libs
        COMMAND sudo ${CMAKE_COMMAND} --build . --target install
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(build_demo
        COMMAND ${CMAKE_COMMAND} --build . --target demo
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(uninstall
        COMMAND sudo ${CMAKE_COMMAND} -P ${CMAKE_EXTERNAL_DIR}/uninstall.cmake
)
########################################### END CUSTOM TARGETS ##########################################

########################################### START BUILD OPTIONS ########################################
option(BUILD_DEMO           "Build the demo"           OFF)
option(BUILD_SHARED_LIBS    "Build shared libraries"   OFF)
option(BUILD_TESTS          "Build tests"              OFF)
########################################### END BUILD OPTIONS ##########################################

########################################### START DEPENDENCIES CHECK ###################################
find_package(Freetype               REQUIRED)
find_package(PNG                    REQUIRED)
find_package(OpenAL                 REQUIRED)
find_package(PkgConfig              REQUIRED)
find_package(nlohmann_json  3.2.0   REQUIRED)
find_package(PkgConfig              REQUIRED)
pkg_check_modules(LIBSIXEL          REQUIRED    libsixel)
pkg_check_modules(SNDFILE           REQUIRED    sndfile)
pkg_check_modules(LIBEVDEV          REQUIRED    libevdev)
########################################### END DEPENDENCIES CHECK #####################################

set( INCLUDE_DIR
        ${TDL_INCLUDE_DIR}
        ${PNG_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS}
        ${SNDFILE_INCLUDE_DIRS}
        ${LIBEVDEV_INCLUDE_DIRS}
        ${LIBSIXEL_INCLUDE_DIRS}
)
list (FILTER INCLUDE_DIR EXCLUDE REGEX ${TDL_SIXEL_INCL_DIR}/.*)
list (FILTER INCLUDE_DIR EXCLUDE REGEX ${TDL_TTY_INCL_DIR}/.*)
list (FILTER INCLUDE_DIR EXCLUDE REGEX ${TDL_ASCII_INCL_DIR}/.*)

file(GLOB_RECURSE TDL_SOURCE ${TDL_SOURCE_DIR}/*.cpp)
list(FILTER TDL_SOURCE EXCLUDE REGEX ${TDL_SIXEL_DIR}/.*)
list(FILTER TDL_SOURCE EXCLUDE REGEX ${TDL_TTY_DIR}/.*)
list(FILTER TDL_SOURCE EXCLUDE REGEX ${TDL_ASCII_DIR}/.*)



include_directories(
        ${TDL_INCLUDE_DIR}
        ${PNG_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS}
        ${SNDFILE_INCLUDE_DIRS}
        ${LIBEVDEV_INCLUDE_DIRS}
        ${LIBSIXEL_INCLUDE_DIRS}
)

message("Building shared libraries: ${BUILD_SHARED_LIBS}")

add_library(TDL SHARED ${TDL_SOURCE})

target_link_libraries(TDL
        ${PNG_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        ${OPENAL_LIBRARY}
        ${SNDFILE_LIBRARIES}
         nlohmann_json::nlohmann_json
        ${LIBEVDEV_LIBRARIES}
        ${LIBSIXEL_LIBRARIES}
)

########################################### START INSTALLATION ###########################################
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/tdl
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")

install(TARGETS TDL
        EXPORT TDLTargets
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include)

add_subdirectory(${TDL_SIXEL_DIR}) # install .so library for sixel
add_subdirectory(${TDL_TTY_DIR}) # install .so library for tty
add_subdirectory(${TDL_ASCII_DIR}) # install .so library for ascii
########################################### END INSTALLATION #############################################

########################################### START EXPORT CONFIG ##########################################
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
########################################### END EXPORT CONFIG ############################################

########################################### START EXPORT ASSET ###########################################
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/include/TDL/Graphics/Widget/Default
        DESTINATION ${CMAKE_BINARY_DIR}/assets)


install(DIRECTORY ${CMAKE_BINARY_DIR}/assets
        DESTINATION share/TDL)

set(INSTALLED_ASSETS_DIR "${CMAKE_INSTALL_PREFIX}/share/TDL/assets")

add_definitions(-DASSETS_DIR="${INSTALLED_ASSETS_DIR}")
########################################### END EXPORT ASSET ##############################################

if (BUILD_TESTS)
    add_subdirectory(${TDL_TEST_DIR})
endif()

if (BUILD_DEMO)
    add_dependencies(build_demo install_libs)
    add_subdirectory(${TDL_EXAMPLES_DIR})
endif()