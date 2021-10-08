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
  QT
  NAMES
    Qt6
    Qt5
  COMPONENTS
    Core5Compat
    Gui
    Widgets
    Xml
    Svg
    Network
    Concurrent
    LinguistTools
  REQUIRED)
find_package(
  Qt${QT_VERSION_MAJOR}
  COMPONENTS
    Core5Compat
    Gui
    Widgets
    Xml
    Svg
    Network
    Concurrent
    LinguistTools
  REQUIRED)

#message("src" ${CMAKE_SOURCE_DIR})

set(TS_FILES ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_en_gb.ts ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_en_us.ts ${CMAKE_SOURCE_DIR}/i18n/${PROJECT_NAME}_de.ts)
#set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION "i18n")
#qt_create_translation(EPubEdit ${CMAKE_SOURCE_DIR} ${TS_FILES} )

find_package(gtest REQUIRED gtest>=1.7.0 PATHS /usr/lib64/cmake/GTest NAMES GTest)
set(gtest_disable_pthreads on)

#find_package(gmock REQUIRED gmock>=1.7.0)

set(MIN_YAMLCPP_VERSION 0.6.0)
find_package(yaml-cpp REQUIRED)

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
