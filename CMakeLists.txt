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

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

find_package(
#  QT5
#  NAMES
    Qt5
  COMPONENTS
    Gui
    Widgets
    Xml
    Svg
    Network
    Concurrent
    LinguistTools
  REQUIRED)
find_package(
#  QT6
#  NAMES
    Qt6
  COMPONENTS
    Core5Compat
    Gui
    Widgets
    Xml
    Svg
    Network
    Concurrent
    LinguistTools
    Test
  REQUIRED)

set(TS_FILES ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_en_gb.ts ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_en_us.ts ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_de.ts)

find_package(gtest REQUIRED gtest>=1.7.0 PATHS /usr/lib64/cmake/GTest NAMES GTest)
set(gtest_disable_pthreads on)

set(MIN_YAMLCPP_VERSION 0.6.0)
find_package(yaml-cpp REQUIRED)

find_library(TIDY tidy)

if (NOT yaml-cpp_FOUND)
  message("yaml-cpp NOT found")
endif()

set(MAKE_APPLICATION TRUE)
# epubedit plugins (future use)
#add_subdirectory(plugins)

# extlibs holds other peoples libraries
add_subdirectory(extlibs)

# libs hold mine
add_subdirectory(libs)

# epubedit general library files
if (MAKE_APPLICATION)
  add_subdirectory(epubedit)
endif()
