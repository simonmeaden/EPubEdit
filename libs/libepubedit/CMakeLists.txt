cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

project(libepubedit LANGUAGES CXX)

add_library(${PROJECT_NAME} SHARED "")
add_library(EPubEdit::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Svg
    QuaZip::QuaZip
    yaml-cpp
    QYamlCpp::qyamlcpp
    tidy
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_sources(${PROJECT_NAME}
#  PUBLIC

  PRIVATE
    src/epubedit.cpp

    src/forms/metadataform.cpp
    src/document/ebookcommon.cpp
    src/document/epubdocument.cpp
    src/document/epubdocument_p.h
    src/document/epubdocument_p.cpp
    src/document/ebookmetadata.cpp
    src/document/authors.cpp
    src/document/library.cpp
    src/document/dcterms.cpp
    src/document/foaf.cpp
    src/document/marcrelator.cpp
    src/document/series.cpp
    src/document/htmlparser.cpp
    src/document/htmltidy.h
    src/document/htmltidy.cpp

    src/options.cpp

    # Utility classes
    src/util/csvsplitter.cpp

)
