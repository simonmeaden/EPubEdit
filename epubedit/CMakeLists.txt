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
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    QuaZip::QuaZip
    QYamlCpp::qyamlcpp
    EPubEdit::libepubedit
)
