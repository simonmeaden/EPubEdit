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

set(TS_FILES ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_en_gb.ts ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_en_us.ts ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_de.ts)

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

)
