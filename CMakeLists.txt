cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0076 NEW)
cmake_policy(SET CMP0077 NEW)
cmake_policy(SET CMP0079 NEW)

project(EPubEdit LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(TS_FILES lang/epubedit_en_GB.ts)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core5Compat Gui Widgets Xml Svg LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core5Compat Gui Widgets Xml Svg LinguistTools REQUIRED)

#set(MIN_Tidy_VERSION 6)
#find_package(Tidy ${MIN_Tidy_VERSION} REQUIRED)

set(MIN_YAMLCPP_VERSION 0.6.0)
find_package(yaml-cpp REQUIRED)

if (NOT yaml-cpp_FOUND)
  message("yaml-cpp NOT found")
endif()

#add_subdirectory(plugins)
add_subdirectory(libs)
add_subdirectory(epubedit)
