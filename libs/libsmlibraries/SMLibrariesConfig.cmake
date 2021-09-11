get_filename_component(SMLIBRARIES_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

list(APPEND CMAKE_MODULE_PATH ${SMLIBRARIES_CMAKE_DIR})

if (DEFINED USE_QT_5)
  find_dependency(Qt5)
elseif (DEFINED USE_QT_6)
  find_dependency(Qt6)
endif()

find_dependency(yaml-cpp 0.6.0)

if(NOT TARGET SMLibraries::SMLibraries)
    include("${SMLIBRARIES_CMAKE_DIR}/SMLibrariesTargets.cmake")
endif()

set(SMWIDGETS_LIBRARIES SMLibraries::SMLibraries)

