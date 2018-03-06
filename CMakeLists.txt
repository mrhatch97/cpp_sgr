cmake_minimum_required (VERSION 3.0.2)

project (cpp_sgr LANGUAGES CXX)

set(VERSION_MAJOR   1)
set(VERSION_MINOR   1)
set(VERSION_PATCH   0)

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS} "-Wextra")

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(demo ${SOURCES})

find_package(Doxygen)

option(BUILD_DOCUMENTATION "Build Doxygen documentation" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
    else(DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen needs to be installed to generate the doxygen documentation")
    endif(DOXYGEN_FOUND)
endif(BUILD_DOCUMENTATION)
