project(libsmlibraries LANGUAGES CXX)

option(BUILD_DOC "Build documentation" ON)

find_package(Doxygen)
if (DOXYGEN_FOUND)
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
    # DOxygen configuration file
    doxygen/Doxyfile.in

    # NEED to include the header files here otherwise MOC won't handle them.
    # they are also automatically included in the target_include_directories 
    # section above

    # internationallanguage classes
    include/languages.h
    src/languages.cpp
    include/unstatistical.h
    src/unstatistical.cpp
    #
    include/filedownloader.h
    src/filedownloader.cpp
    # html tidy wrapper.
    include/qtidydoc.h
    src/qtidydoc.cpp

    # labelled classes
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

    # dockwidget classes
    include/docktypes.h

    include/abstractdockwidget.h
    src/abstractdockwidget.cpp
    include/private/abstractdockwidgetprivate.h
    src/private/abstractdockwidgetprivate.cpp

    include/dockwidget.h
    src/dockwidget.cpp
    include/private/dockwidgetprivate.h
    src/private/dockwidgetprivate.cpp

    include/abstractdockitem.h
    src/abstractdockitem.cpp
    src/private/abstractdockitemprivate.h
    src/private/abstractdockitemprivate.cpp

    include/dockheader.h
    src/dockheader.cpp

    include/dockfooter.h
    src/dockfooter.cpp

    include/docktoolbar.h
    src/docktoolbar.cpp

    include/dockcorner.h
    src/dockcorner.cpp

    include/headerwidget.h
    src/headerwidget.cpp
    include/private/headerwidgetprivate.h
    src/private/headerwidgetprivate.cpp

    include/footerwidget.h
    src/footerwidget.cpp
    include/private/footerwidgetprivate.h
    src/private/footerwidgetprivate.cpp

    include/headerfooterwidget.h
    src/headerfooterwidget.cpp
    include/private/headerfooterwidgetprivate.h
    src/private/headerfooterwidgetprivate.cpp

    include/widgetitem.h
    src/widgetitem.cpp
    src/private/widgetitemprivate.cpp
    src/private/widgetitemprivate.h

    include/seperatorwidget.h
    src/seperatorwidget.cpp
    src/private/seperatorwidgetprivate.cpp
    src/private/seperatorwidgetprivate.h

    include/labelwidget.h
    src/labelwidget.cpp
    src/private/labelwidgetprivate.cpp
    src/private/labelwidgetprivate.h

    include/buttonwidget.h
    src/buttonwidget.cpp
    src/private/buttonwidgetprivate.cpp
    src/private/buttonwidgetprivate.h

    include/docklistitem.h
    src/docklistitem.cpp

    include/listbuttonwidget.h
    src/listbuttonwidget.cpp
    src/private/listbuttonwidgetprivate.cpp
    src/private/listbuttonwidgetprivate.h

    include/listwidget.h
    src/listwidget.cpp
    src/private/listwidgetprivate.cpp
    src/private/listwidgetprivate.h

    include/docksplitter.h
    src/docksplitter.cpp
    # multisplitter
    include/multisplitter.h
    src/multisplitter.cpp
    # borderlayout Qt example
    include/borderlayout.h
    src/borderlayout.cpp

    # tinyxml library
    include/tinyxml2.h
    src/tinyxml2.cpp
    include/tinyxmldocument.h
    src/tinyxmldocument.cpp

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
