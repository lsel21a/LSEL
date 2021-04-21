FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)

idf_component_register(SRCS ${app_sources})
set(EXTRA_COMPONENT_DIRS ./components)
# set(EXTRA_COMPONENT_DIRS ${CMAKE_SOURCE_DIR}/external/esp-idf-lib/components)
