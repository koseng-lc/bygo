cmake_minimum_required(VERSION 2.8.3)
project(bygo)

# Include current CMake dir
include_directories(${PROJECT_SOURCE_DIR})

add_library(bygo_lib
    bygo/bygo.hpp
)
set_target_properties(bygo_lib PROPERTIES LINKER_LANGUAGE CXX)

# Specifying the include dir for build and install
target_include_directories(bygo_lib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${BYGO_INCLUDE_PATH}>
)

# Add current target lib into the export group
install(TARGETS bygo_lib
    EXPORT ${PROJECT_NAME}_export ARCHIVE DESTINATION ${BYGO_LIB_PATH}
)

# Export current include directory (which contains headers) into the install path
install(DIRECTORY ${PROJECT_SOURCE_DIR}/bygo/ DESTINATION ${BYGO_HEADER_PATH})