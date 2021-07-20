cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

project(epubedit LANGUAGES CXX)

add_executable(${PROJECT_NAME} "")

target_compile_options(${PROJECT_NAME} PRIVATE -Werror)

target_sources(${PROJECT_NAME}
  PRIVATE
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp

    src/forms/metadataform.h
    src/forms/metadataform.cpp
    src/forms/epubedit.h
    src/forms/epubedit.cpp
    src/forms/epubeditor.h
    src/forms/epubeditor.cpp
    src/forms/configurationeditor.cpp
    src/forms/config.cpp

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
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Network
)
