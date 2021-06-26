cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

project(EPubEdit LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

set(TS_FILES lang/epubedit_en_GB.ts)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core5Compat Gui Widgets Xml Svg LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core5Compat Gui Widgets Xml Svg LinguistTools REQUIRED)

set(MIN_Tidy_VERSION 6)
find_package(Tidy ${MIN_Tidy_VERSION} REQUIRED)

set(MIN_YAMLCPP_VERSION 0.6.0)
find_package(yaml-cpp REQUIRED)

if (NOT yaml-cpp_FOUND)
  message("yaml-cpp NOT found")
endif()

add_library(libepubedit SHARED "")
add_library(EPubEdit::epubedit ALIAS libepubedit)

set(QUAZIP_QT_MAJOR_VERSION ${QT_VERSION_MAJOR})
add_subdirectory(quazip)

target_compile_options(libepubedit PRIVATE -Werror)
target_compile_features(libepubedit PRIVATE cxx_std_17)
set_target_properties(libepubedit PROPERTIES DEBUG_POSTFIX "d")

target_link_libraries(libepubedit
  PRIVATE
    Qt${QT_VERSION_MAJOR}::Core5Compat
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Svg
    QuaZip::QuaZip
    yaml-cpp
    tidy
)

target_include_directories(libepubedit
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_sources(libepubedit

  PRIVATE
    src/epubedit.cpp

    src/forms/metadataform.h
    src/forms/metadataform.cpp

    src/document/epubdocument.h
    src/document/epubdocument.cpp
    src/document/epubpackage.h
    src/document/epubpackage.cpp
    src/document/epubcontainer.h
    src/document/epubcontainer.cpp
    src/document/epubmetadata.h
    src/document/epubmetadata.cpp
    src/document/epubtitle.h
    src/document/epubtitle.cpp
    src/document/epubauthor.h
    src/document/epubauthor.cpp

    src/document/htmltidy.h
    src/document/htmltidy.cpp

    src/options.cpp

    # QYamlCpp
    src/qyamlcpp/collection.h
    src/qyamlcpp/comment.h
    src/qyamlcpp/emitter.h
    src/qyamlcpp/node.h
    src/qyamlcpp/parse.h
    src/qyamlcpp/qyamlcpp.h

    # Utility classes
    src/util/csvsplitter.cpp

)

add_subdirectory(epubedit)
#add_subdirectory(plugins)
