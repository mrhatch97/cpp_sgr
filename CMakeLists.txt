cmake_minimum_required (VERSION 3.8)

project (cpp_sgr
  VERSION 1.2.0
  LANGUAGES CXX
)

find_package(Doxygen)

option(BUILD_DOCUMENTATION "Build Doxygen documentation" ${DOXYGEN_FOUND})
option(BUILD_DEMO "Build SGR demo" ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(cpp_sgr INTERFACE)

target_compile_features(cpp_sgr
  INTERFACE
    cxx_std_11)

target_include_directories(cpp_sgr
  INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(BUILD_DEMO)
  add_executable(demo
    src/demo.cpp)

  target_link_libraries(demo
    cpp_sgr)
endif()

install(TARGETS cpp_sgr EXPORT cpp_sgrConfig
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

write_basic_package_version_file(
  cpp_sgrConfigVersion.cmake
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(TARGETS cpp_sgr
  FILE cpp_sgrConfig.cmake
  NAMESPACE cpp_sgr::
)

install(EXPORT cpp_sgrConfig
  DESTINATION share/cpp_sgr/cmake
)

if(BUILD_DOCUMENTATION)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build enabled")

        add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
    else(DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen needs to be installed to generate the doxygen documentation")
    endif(DOXYGEN_FOUND)
endif(BUILD_DOCUMENTATION)
