
qt_wrap_cpp(MOC_Files
  ../../include/widgets/lnplaintextedit.h
  ../../include/widgets/lntextedit.h
  ../../include/widgets/helpdialogs.h
  ../../include/widgets/htmlscanner.h
  ../../include/widgets/htmlhighlighter.h
  ../../include/widgets/htmltypes.h
  ../../include/widgets/extendedcolordialog.h
  ../../include/widgets/hoverwidget.h
)

target_sources(
    ${PROJECT_NAME}
  PRIVATE
  # NEED to include the header files here otherwise MOC won't handle them.
  # they are also automatically included in the target_include_directories
  # section but moc and indeed QtCreator seem to need them here as well
  ../../include/widgets/lnplaintextedit.h
  ../../include/widgets/lntextedit.h
  ../../include/widgets/helpdialogs.h
  ../../include/widgets/htmlscanner.h
  ../../include/widgets/htmlhighlighter.h
  ../../include/widgets/htmltypes.h
  ../../include/widgets/extendedcolordialog.h
  ../../include/widgets/hoverwidget.h
  #

  # line number edits
  lnplaintextedit.cpp
  lntextedit.cpp
  iepubeditor.h
  iepubeditor.h

  # help dialogs.
  hoverwidget.cpp

  # HTML 5 scanner/highlighter
  htmlscanner.cpp
  htmlhighlighter.cpp
  htmltypes.cpp

  # extended color dialog
  extendedcolordialog.cpp

  widgets/icons.qrc
  widgets/fonts.qrc
)
