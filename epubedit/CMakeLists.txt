cmake_minimum_required(VERSION 3.5)
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

    src/forms/metadataform.h
    src/forms/metadataform.cpp
    src/forms/undocommands.h
    src/forms/undocommands.cpp
    src/forms/titleview.h
    src/forms/titleview.cpp
    src/forms/metadatalist.h
    src/forms/metadatalist.cpp
#    src/forms/epubedit.h
#    src/forms/epubedit.cpp
    src/forms/epubeditor.h
    src/forms/epubeditor.cpp
    src/forms/epubcontents.h
    src/forms/epubcontents.cpp
    src/forms/mainwidget.h
    src/forms/mainwidget.cpp
    src/forms/centralwidget.h
    src/forms/centralwidget.cpp
    src/forms/infowidget.h
    src/forms/infowidget.cpp

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
    EPubEdit::libepubedit
    SMLibraries::libsmlibraries
    QuaZip::QuaZip
    QYamlCpp::qyamlcpp
    yaml-cpp
#    tidy
    Qt::Gui
    Qt::Widgets
    Qt::Xml
    Qt::Network
)
