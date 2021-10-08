project(libsmlibraries LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC "")
add_library(SMLibraries::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

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
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(TS_FILES ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_en_gb.ts ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_en_us.ts ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_de.ts)

target_sources(${PROJECT_NAME}
#  PUBLIC

  PRIVATE
    # NEED to include the header files here otherwise MOC won't handle them.
    # they are also automatically included in the target_include_directories 
    # section above
    include/languages.h
    src/languages.cpp
    include/unstatistical.h
    src/unstatistical.cpp
#    include/iso639codes.h
#    src/iso639codes.cpp
    include/filedownloader.h
    src/filedownloader.cpp
    include/abstractlabelledspinbox.h
    src/abstractlabelledspinbox.cpp
    include/abstractlabelledwidget.h
    src/abstractlabelledwidget.cpp
    include/exspinbox.h
    src/exspinbox.cpp
    include/extendedcolordialog.h
    src/extendedcolordialog.cpp
    include/extendedcolordialog.h
    src/extendedcolordialog.cpp
    include/hexformatdelegate.h
    src/hexformatdelegate.cpp
    include/labelledcombobox.h
    src/labelledcombobox.cpp
    include/labelleddoublespinbox.h
    src/labelleddoublespinbox.cpp
    include/labelledexspinbox.h
    src/labelledexspinbox.cpp
    include/labelledlineedit.h
    src/labelledlineedit.cpp
    include/labelledspinbox.h
    src/labelledspinbox.cpp
    include/labelledtextfield.h
    src/labelledtextfield.cpp
    src/sm_widgets_global.h
    include/x11colors.h
    src/x11colors.cpp
    include/paths.h
    src/paths.cpp
    include/helpdialogs.h
    src/helpdialogs.cpp
    include/stringutil.h
    src/stringutil.cpp
)

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
