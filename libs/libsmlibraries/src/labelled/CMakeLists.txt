
#qt_wrap_cpp(MOC_Files
#  ../../include/labelled/abstractlabelledspinbox.h
#  ../../include/labelled/abstractlabelledwidget.h
#  ../../include/labelled/exspinbox.h
#  ../../include/labelled/hexformatdelegate.h
#  ../../include/labelled/labelledcombobox.h
#  ../../include/labelled/labelleddoublespinbox.h
#  ../../include/labelled/labelledexspinbox.h
#  ../../include/labelled/labelledlineedit.h
#  ../../include/labelled/labelledspinbox.h
#  ../../include/labelled/labelledtextfield.h

#)

#add_subdirectory(translate)

target_sources(
    ${PROJECT_NAME}
  PRIVATE
  # NEED to include the header files here otherwise MOC won't handle them.
  # they are also automatically included in the target_include_directories
  # section above
  ../../include/labelled/abstractlabelledspinbox.h
  ../../include/labelled/abstractlabelledwidget.h
  ../../include/labelled/exspinbox.h
  ../../include/labelled/hexformatdelegate.h
  ../../include/labelled/labelledcombobox.h
  ../../include/labelled/labelleddoublespinbox.h
  ../../include/labelled/labelledexspinbox.h
  ../../include/labelled/labelledlineedit.h
  ../../include/labelled/labelledspinbox.h
  ../../include/labelled/labelledtextfield.h
  #


  # labelled classes
  abstractlabelledspinbox.cpp
  abstractlabelledwidget.cpp
  exspinbox.cpp
  hexformatdelegate.cpp
  labelledcombobox.cpp
  labelleddoublespinbox.cpp
  labelledexspinbox.cpp
  labelledlineedit.cpp
  labelledspinbox.cpp
  labelledtextfield.cpp

)
