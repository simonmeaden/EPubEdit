
message("Qt Version:" ${PROJECT_QT})

add_executable(examples
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
)

target_link_libraries(examples
  LINK_PUBLIC
    smlibraries
)
message ("SMlibraries found : " ${smlibraries_FOUND})

target_link_libraries(examples
  PRIVATE
    Qt::Core Qt::Gui Qt::Widgets
)

target_include_directories(examples
  PRIVATE
    ../include
)
