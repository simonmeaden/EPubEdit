cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

project(epubedit LANGUAGES CXX)

add_executable(${PROJECT_NAME} "")

target_compile_options(${PROJECT_NAME} PRIVATE -Werror)

set(TS_FILES ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_en_gb.ts ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_en_us.ts ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_de.ts)

target_sources(${PROJECT_NAME}
  PRIVATE
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp

    # NEED to include the header files her otherwise MOC won't handle them.
    include/document/bookpointers.h
    include/document/bookstructs.h
    include/document/bookenums.h
    include/document/epubdocument.h
    src/document/epubdocument.cpp
    include/document/metadata.h
    src/document/metadata.cpp
    include/document/authors.h
    src/document/authors.cpp
    include/document/library.h
    src/document/library.cpp
    include/document/sharedbookdata.h
    src/document/sharedbookdata.cpp
    include/document/sharedauthordata.h
    src/document/sharedauthordata.cpp
    include/document/options.h
    src/document/options.cpp
    include/document/dcterms.h
    src/document/dcterms.cpp
    include/document/foaf.h
    src/document/foaf.cpp
    include/document/marcrelator.h
    src/document/marcrelator.cpp
    include/document/series.h
    src/document/series.cpp
#    src/document/htmlparser.cpp
#    src/document/htmltidy.cpp
    include/document/uniquestring.h
    src/document/uniquestring.cpp

    src/forms/metadataform.h
    src/forms/metadataform.cpp
    src/forms/undocommands.h
    src/forms/undocommands.cpp
    src/forms/metadatalist.h
    src/forms/metadatalist.cpp
    src/forms/emultisplitter.h
    src/forms/emultisplitter.cpp

    src/forms/epubcontents.h
    src/forms/epubcontents.cpp
    src/forms/mainwidget.h
    src/forms/mainwidget.cpp
    src/forms/editorwidget.h
    src/forms/editorwidget.cpp
    src/forms/leftsidebar.h
    src/forms/leftsidebar.cpp
    src/forms/rightsidebar.h
    src/forms/rightsidebar.cpp
    src/forms/infowidget.h
    src/forms/infowidget.cpp

    src/forms/iepubeditor.h
    src/forms/epubedit.h
    src/forms/epubedit.cpp

    src/forms/codeedit.h
    src/forms/codeedit.cpp
#    src/forms/private/codeeditorprivate.h
#    src/forms/private/codeeditorprivate.cpp

    src/forms/imageedit.h
    src/forms/imageedit.cpp
#    src/forms/private/imageeditprivate.h
#    src/forms/private/imageeditprivate.cpp

src/forms/epubeditor.h
    src/forms/epubeditor.cpp
    src/forms/private/epubeditorprivate.h
    src/forms/private/epubeditorprivate.cpp

    src/forms/filedialog.h
    src/forms/filedialog.cpp

    src/forms/abstractepubeditor.h
    src/forms/abstractepubeditor.cpp
    src/forms/private/abstractepubeditorprivate.h
    src/forms/private/abstractepubeditorprivate.cpp

    src/forms/twocolview.h
    src/forms/twocolview.cpp
    src/forms/threecolview.h
    src/forms/threecolview.cpp
    src/forms/common.cpp
    src/forms/navitem.h
    src/forms/navitem.cpp
    src/forms/tocform.h
    src/forms/tocform.cpp
    src/forms/common.h

    include/config.h
    src/config.cpp
    include/forms/configurationeditor.h
    src/forms/configurationeditor.cpp
    src/forms/languagetagbuilderdialogresource.qrc
    include/forms/languagetagbuilderdialog.h
    src/forms/languagetagbuilderdialog.cpp

    # Utility classes
    src/util/csvsplitter.h
    src/util/csvsplitter.cpp

    src/images.qrc

)

target_include_directories(${PROJECT_NAME}
    PUBLIC
      $<INSTALL_INTERFACE:include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    SMLibraries::libsmlibraries
    QuaZip::QuaZip
    QYamlCpp::qyamlcpp
    yaml-cpp
    Qt::Gui
    Qt::Widgets
    Qt::Xml
    Qt::Network
    CuteLogger
#    Log4Qt
)
