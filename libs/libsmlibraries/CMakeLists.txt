cmake_minimum_required(VERSION 3.14)

project(libsmlibraries LANGUAGES CXX)

option(BUILD_DOC "Build documentation" ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MIN_YAMLCPP_VERSION 0.6.0)
find_package(yaml-cpp REQUIRED)

if (NOT DEFINED EPUBEDIT_BUILD)
  find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Network Xml Svg Concurrent LinguistTools)
  if (QT_VERSION_MAJOR EQUAL 5)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Network Xml Svg Concurrent LinguistTools)
  elseif (QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Network Xml Svg Concurrent LinguistTools Core5Compat)
  endif()
endif()

find_package(Doxygen)
if (DOXYGEN_FOUND)
  # Only build documentation when building in Release mode
  if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
  endif()
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

add_library(${PROJECT_NAME} STATIC "")
add_library(SMLibraries::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Svg
    Qt${QT_VERSION_MAJOR}::Network
    QuaZip::QuaZip
    yaml-cpp
    QYamlCpp::qyamlcpp
    gtest
    ${TIDY}
    CuteLogger
)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


add_subdirectory(src)
add_subdirectory(tests)

#message("SM Qt major version " ${QT_MAJOR_VERSION})
#message("SM Qt interface version " ${INTERFACE_QT_MAJOR_VERSION})

########################################################################
## The section includes the install section.
########################################################################

# Not working needs QuaZip and qyamlcpp in export set so need to work that out.

#include(GNUInstallDirs)

#install(TARGETS ${PROJECT_NAME}
#    EXPORT ${PROJECT_NAME}-targets
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#)
##This is required so that the exported target has the name SMLibraries and not smlibraries
#set_target_properties(${PROJECT_NAME} PROPERTIES EXPORT_NAME SMLibraries)

#install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#install(EXPORT ${PROJECT_NAME}-targets
#  FILE
#    SMLibrariesTargets.cmake
#  NAMESPACE
#    SMLibraries::
#  DESTINATION
#    ${CMAKE_INSTALL_LIBDIR}/cmake/SMLibraries
#)

##Create a ConfigVersion.cmake file
#set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/SMLibraries)
#include(CMakePackageConfigHelpers)
#write_basic_package_version_file(
#    ${CMAKE_CURRENT_BINARY_DIR}/SMLibrariesConfigVersion.cmake
#    VERSION ${PROJECT_VERSION}
#    COMPATIBILITY AnyNewerVersion
#)

#configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/SMLibrariesConfig.cmake.in
#    ${CMAKE_CURRENT_BINARY_DIR}/SMLibrariesConfig.cmake
#    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
#)

##Install the config, configversion and custom find modules
#install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/SMLibrariesConfig.cmake
#    ${CMAKE_CURRENT_BINARY_DIR}/SMLibrariesConfigVersion.cmake
#    DESTINATION ${INSTALL_CONFIGDIR}
#)

###############################################
### Exporting from the build tree
#export(EXPORT ${PROJECT_NAME}-targets
#    FILE ${CMAKE_CURRENT_BINARY_DIR}/SMLibrariesTargets.cmake
#    NAMESPACE SMLibraries::)

## install additional files
#set(INSTALL_INCLUDE_DIR "include")
#install(FILES ${SMWIDGETS_EXTRAS}
#  DESTINATION ${INSTALL_INCLUDE_DIR}/sm_widgets
#  CONFIGURATIONS Release
#  )
#install(FILES ${QYAMLCPP_EXTRAS}
#  DESTINATION ${INSTALL_INCLUDE_DIR}/qyamlcpp
#  CONFIGURATIONS Release
#  )

##Register package in the User Package Registry
#export(PACKAGE SMLibraries)
