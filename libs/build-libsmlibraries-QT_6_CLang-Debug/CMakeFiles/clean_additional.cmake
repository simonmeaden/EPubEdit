# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/libsmlibraries_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/libsmlibraries_autogen.dir/ParseCache.txt"
  "libsmlibraries_autogen"
  "tests/testlibsmlibraries/CMakeFiles/smlibtest_autogen.dir/AutogenUsed.txt"
  "tests/testlibsmlibraries/CMakeFiles/smlibtest_autogen.dir/ParseCache.txt"
  "tests/testlibsmlibraries/smlibtest_autogen"
  )
endif()
