# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "epubedit/CMakeFiles/epubedit_autogen.dir/AutogenUsed.txt"
  "epubedit/CMakeFiles/epubedit_autogen.dir/ParseCache.txt"
  "epubedit/epubedit_autogen"
  "extlibs/CuteLogger/CMakeFiles/CuteLogger_autogen.dir/AutogenUsed.txt"
  "extlibs/CuteLogger/CMakeFiles/CuteLogger_autogen.dir/ParseCache.txt"
  "extlibs/CuteLogger/CuteLogger_autogen"
  "extlibs/quazip/quazip/CMakeFiles/QuaZip_autogen.dir/AutogenUsed.txt"
  "extlibs/quazip/quazip/CMakeFiles/QuaZip_autogen.dir/ParseCache.txt"
  "extlibs/quazip/quazip/QuaZip_autogen"
  "extlibs/quazip/qztest/CMakeFiles/qztest_autogen.dir/AutogenUsed.txt"
  "extlibs/quazip/qztest/CMakeFiles/qztest_autogen.dir/ParseCache.txt"
  "extlibs/quazip/qztest/qztest_autogen"
  "libs/libsmlibraries/CMakeFiles/libsmlibraries_autogen.dir/AutogenUsed.txt"
  "libs/libsmlibraries/CMakeFiles/libsmlibraries_autogen.dir/ParseCache.txt"
  "libs/libsmlibraries/libsmlibraries_autogen"
  "libs/libsmlibraries/tests/testlibsmlibraries/CMakeFiles/smlibtest_autogen.dir/AutogenUsed.txt"
  "libs/libsmlibraries/tests/testlibsmlibraries/CMakeFiles/smlibtest_autogen.dir/ParseCache.txt"
  "libs/libsmlibraries/tests/testlibsmlibraries/smlibtest_autogen"
  )
endif()
