add_executable(epubedit "")

#if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#  message("using Qt >= 6")
#  add_executable(epubedit "")
#  qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
#else()
#    if(ANDROID)
#      message("using Qt < 6 Android")
#      add_library(libepubedit SHARED "")
#    else()
#      message("using Qt < 6")
#      add_executable(libepubedit "")
#    endif()

#    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
#endif()

target_compile_options(epubedit PRIVATE -Werror)

target_sources(epubedit
  PRIVATE
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
)

target_include_directories(epubedit
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../src

)

target_link_libraries(epubedit
  PRIVATE
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    QuaZip::QuaZip
    libepubedit
)
