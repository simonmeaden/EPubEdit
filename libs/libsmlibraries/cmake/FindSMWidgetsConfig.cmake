
get_filename_component(SMLIBRARIES_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

find_dependency(yamlcpp @MIN_YAMLCPP_VERSION@)

if(NOT TARGET SMLibraries::smlibraries)
    include("${SMLIBRARIES_CMAKE_DIR}/qyamlcppConfigTargets.cmake")
endif()
