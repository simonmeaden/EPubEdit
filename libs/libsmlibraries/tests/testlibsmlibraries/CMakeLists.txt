add_executable(smlibtest
  main.cpp
  testbcp47languages.cpp
)

target_link_libraries(smlibtest
  PRIVATE
    SMLibraries::libsmlibraries
    QuaZip::QuaZip
    QYamlCpp::qyamlcpp
    yaml-cpp

    gtest
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::Network
)
