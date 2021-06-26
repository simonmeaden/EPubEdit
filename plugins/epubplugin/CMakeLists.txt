add_library(epubplugin SHARED "")

target_sources(epubplugin
  PRIVATE
    src/epubplugin.h
    src/epubplugin.cpp
    src/epubplugin_global.h
)

target_include_directories(epubplugin
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(epubplugin
  PRIVATE
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Svg
    libepubedit
)
