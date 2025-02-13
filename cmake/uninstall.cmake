if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: ${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt")
endif()

file(READ "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt" files)
string(REPLACE "\n" ";" files "${files}")

foreach(file ${files})
    message(STATUS "Uninstalling ${file}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove "${file}")
endforeach()