cmake_minimum_required(VERSION 3.5)
project(qyamlcpp LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(QYamlCpp::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
  INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${PROJECT_NAME} INTERFACE yaml-cpp)
