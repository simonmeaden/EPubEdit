cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0079 NEW)

project(epubedit LANGUAGES CXX)

add_executable(${PROJECT_NAME} "")

target_compile_options(${PROJECT_NAME} PRIVATE -Werror)

target_include_directories(${PROJECT_NAME}
    PUBLIC
      $<INSTALL_INTERFACE:include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory(src)

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
