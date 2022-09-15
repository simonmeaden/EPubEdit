set(CMAKE_AUTOMOC ON)

#qt_wrap_cpp(MOC_Files
#  ../../include/docker/abstractdockwidget.h
#  ../../include/docker/dockwidget.h
#  ../../include/docker/abstractdockitem.h
#  ../../include/docker/dockheader.h
#  ../../include/docker/dockfooter.h
#  ../../include/docker/docktoolbar.h
#  ../../include/docker/dockcorner.h
#  ../../include/docker/headerwidget.h
#  ../../include/docker/footerwidget.h
#  ../../include/docker/widgetitem.h
#  ../../include/docker/seperatorwidget.h
#  ../../include/docker/labelwidget.h
#  ../../include/docker/buttonwidget.h
#  ../../include/docker/docklistitem.h
#  ../../include/docker/listbuttonwidget.h
#  ../../include/docker/listwidget.h
#  ../../include/docker/docksplitter.h
#)

#add_subdirectory(translate)

target_sources(
    ${PROJECT_NAME}
  PRIVATE
  # NEED to include the header files here otherwise MOC won't handle them.
  # they are also automatically included in the target_include_directories
  # section above
  ../../include/docker/abstractdockwidget.h
  ../../include/docker/dockwidget.h
  ../../include/docker/abstractdockitem.h
  ../../include/docker/dockheader.h
  ../../include/docker/dockfooter.h
  ../../include/docker/docktoolbar.h
  ../../include/docker/dockcorner.h
  ../../include/docker/headerwidget.h
  ../../include/docker/footerwidget.h
  ../../include/docker/widgetitem.h
  ../../include/docker/seperatorwidget.h
  ../../include/docker/labelwidget.h
  ../../include/docker/buttonwidget.h
  ../../include/docker/docklistitem.h
  ../../include/docker/listbuttonwidget.h
  ../../include/docker/listwidget.h
  ../../include/docker/docksplitter.h
  #

  # dockwidget classes
  abstractdockwidget.cpp
  private/abstractdockwidgetprivate.h
  private/abstractdockwidgetprivate.cpp

  dockwidget.cpp
  private/dockwidgetprivate.h
  private/dockwidgetprivate.cpp

  abstractdockitem.cpp
  private/abstractdockitemprivate.h
  private/abstractdockitemprivate.cpp

  dockheader.cpp
  dockfooter.cpp
  docktoolbar.cpp

  dockcorner.cpp

  headerwidget.cpp
  private/headerwidgetprivate.h
  private/headerwidgetprivate.cpp

  footerwidget.cpp
  private/footerwidgetprivate.h
  private/footerwidgetprivate.cpp

  headerfooterwidget.cpp
  private/headerfooterwidgetprivate.h
  private/headerfooterwidgetprivate.cpp

  widgetitem.cpp
  private/widgetitemprivate.cpp
  private/widgetitemprivate.h

  seperatorwidget.cpp
  private/seperatorwidgetprivate.cpp
  private/seperatorwidgetprivate.h

  labelwidget.cpp
  private/labelwidgetprivate.cpp
  private/labelwidgetprivate.h

  buttonwidget.cpp
  private/buttonwidgetprivate.cpp
  private/buttonwidgetprivate.h

  docklistitem.cpp

  listbuttonwidget.cpp
  private/listbuttonwidgetprivate.cpp
  private/listbuttonwidgetprivate.h

  listwidget.cpp
  private/listwidgetprivate.cpp
  private/listwidgetprivate.h

  docksplitter.cpp
)
