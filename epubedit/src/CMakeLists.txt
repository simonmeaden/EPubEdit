
add_subdirectory(document)

set(TS_FILES
  ${CMAKE_SOURCE_DIR}/translate/${PROJECT_NAME}_en_gb.ts
  ${CMAKE_SOURCE_DIR}/translate/${PROJECT_NAME}_en_us.ts
  ${CMAKE_SOURCE_DIR}/translate/${PROJECT_NAME}_de.ts
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
      $<INSTALL_INTERFACE:include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_sources(${PROJECT_NAME}
  PRIVATE
  # NEED to include the header files here otherwise MOC won't handle them.
  # they are also automatically included in the target_include_directories
  # section above
  ../include/config.h
  ../include/forms/configurationeditor.h
  ../include/forms/languagetagbuilderdialog.h
  ../include/document/epubdocument.h
  ../include/widgets/epubedit.h
  ../include/widgets/codeedit.h
  ../include/widgets/imageedit.h

  #
    main.cpp
    mainwindow.h
    mainwindow.cpp

#    basicconfig.cpp
    config.cpp
    forms/configurationeditor.cpp

    document/epubdocument.cpp
    document/metadata.cpp
    document/authors.cpp
    document/library.cpp
    document/sharedbookdata.cpp
    document/sharedauthordata.cpp
    document/options.cpp
    document/dcterms.cpp
    document/foaf.cpp
    document/marcrelator.cpp
    document/series.cpp
    document/uniquestring.cpp

    forms/metadataform.h
    forms/metadataform.cpp
    forms/undocommands.h
    forms/undocommands.cpp
    forms/metadatalist.h
    forms/metadatalist.cpp
    forms/emultisplitter.h
    forms/emultisplitter.cpp

    forms/epubcontents.h
    forms/epubcontents.cpp
    forms/mainwidget.h
    forms/mainwidget.cpp
    forms/editorwidget.h
    forms/editorwidget.cpp
    forms/leftsidebar.h
    forms/leftsidebar.cpp
    forms/rightsidebar.h
    forms/rightsidebar.cpp
    forms/infowidget.h
    forms/infowidget.cpp

    widgets/imageedit.cpp

    widgets/epubedit.cpp
    widgets/codeedit.cpp

    forms/epubeditor.h
    forms/epubeditor.cpp

    forms/filedialog.h
    forms/filedialog.cpp

    forms/abstractepubeditor.h
    forms/abstractepubeditor.cpp

    forms/twocolview.h
    forms/twocolview.cpp
    forms/threecolview.h
    forms/threecolview.cpp
    forms/common.cpp
    forms/navitem.h
    forms/navitem.cpp
    forms/tocform.h
    forms/tocform.cpp
    forms/common.h

    forms/languagetagbuilderdialogresource.qrc
    forms/languagetagbuilderdialog.cpp

    images.qrc

)
