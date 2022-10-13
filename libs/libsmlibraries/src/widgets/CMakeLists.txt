
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
  ../../include/widgets/isettingspage.h
  ../../include/widgets/extendedcolordialog.h
  ../../include/widgets/hoverwidget.h
  ../../include/widgets/lnsettingsdialog.h
  ../../include/widgets/lnstatusbar.h
   #

  # line number edits
  lnplaintextedit.cpp
  lntextedit.cpp
  lnsettingsdialog.cpp
  lnstatusbar.cpp
  isettingspage.cpp
#  iepubeditor.h
  extendedcolordialog.cpp

  # help dialogs.
  helpdialogs.cpp
  hoverwidget.cpp

  # HTML 5 scanner/highlighter
  htmlscanner.cpp
  htmlhighlighter.cpp
  htmltypes.cpp

  # extended color dialog
  extendedcolordialog.cpp

  icons.qrc
  fonts.qrc
)
