# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "quazip/quazip/CMakeFiles/QuaZip_autogen.dir/AutogenUsed.txt"
  "quazip/quazip/CMakeFiles/QuaZip_autogen.dir/ParseCache.txt"
  "quazip/quazip/QuaZip_autogen"
  "quazip/qztest/CMakeFiles/qztest_autogen.dir/AutogenUsed.txt"
  "quazip/qztest/CMakeFiles/qztest_autogen.dir/ParseCache.txt"
  "quazip/qztest/qztest_autogen"
  )
endif()
