
target_sources(
    ${PROJECT_NAME}
  PRIVATE
  # NEED to include the header files here otherwise MOC won't handle them.
  # they are also automatically included in the target_include_directories
  # section but moc and indeed QtCreator seem to need them here as well
  ../../include/utilities/qtidydoc.h
  ../../include/utilities/tinyxml2.h
  ../../include/utilities/tinyxmldocument.h
  ../../include/utilities/stringutil.h
  ../../include/utilities/csvsplitter.h
  ../../include/utilities/x11colors.h
  ../../include/utilities/paths.h
  ../../include/utilities/filedownloader.h
  ../../include/utilities/keymap.h  #
  ../../include/utilities/characters.h

  # html tidy wrapper.
  qtidydoc.cpp

  # tinyxml library
  tinyxml2.cpp
  tinyxmldocument.cpp

  # Utilities
  stringutil.cpp
  csvsplitter.cpp
#  x11colors.cpp
  paths.cpp
  filedownloader.cpp
  keymap.cpp

)
