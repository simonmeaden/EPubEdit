#cmake_minimum_required(VERSION 3.5)
#cmake_policy(SET CMP0076 NEW)
#cmake_policy(SET CMP0079 NEW)

project(libepubedit LANGUAGES CXX)

#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC "")
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
    Qt${QT_VERSION_MAJOR}::Network
    QuaZip::QuaZip
    yaml-cpp
    QYamlCpp::qyamlcpp
    SMLibraries::libsmlibraries
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
    # NEED to include the header files her otherwise MOC won't handle them.
    include/document/epubdocument.h
    src/document/epubdocument.cpp
    include/document/epubmetadata.h
    src/document/epubmetadata.cpp
    include/document/authors.h
    src/document/authors.cpp
    include/document/library.h
    src/document/library.cpp
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

    include/config.h
    src/config.cpp
    include/forms/configurationeditor.h
    src/forms/configurationeditor.cpp
    include/options.h
    src/options.cpp

    # Utility classes
    src/util/csvsplitter.h
    src/util/csvsplitter.cpp

)
