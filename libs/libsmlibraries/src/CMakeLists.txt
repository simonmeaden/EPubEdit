
add_subdirectory(translate)
add_subdirectory(docker)
add_subdirectory(labelled)
add_subdirectory(widgets)
add_subdirectory(utilities)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_sources(
    ${PROJECT_NAME}
  PRIVATE
  # NEED to include the header files here otherwise MOC won't handle them.
  # they are also automatically included in the target_include_directories
  # section above
  ../include/language/languages.h
  ../include/language/unstatistical.h
  ../include/sm_widgets_global.h
  ../include/single/singlemainwindow.h
  ../include/single/qsinglecoreapplication.h
  ../include/multisplitter/multisplitter.h
  ../include/multisplitter/cloneableinterface.h
  ../include/layout/borderlayout.h
  #
  # international language classes
  language/languages.cpp
  language/unstatistical.cpp

  sm_widgets_global.h

  # single applications
  single/singlemainwindow.cpp
  single/qsinglecoreapplication.cpp

  # multisplitter
  multisplitter/multisplitter.cpp
  # borderlayout Qt example
  layout/borderlayout.cpp
)
