cmake_minimum_required(VERSION 2.8.3)
project(bygo_main)

# Enable C++ specific version features
add_compile_options(-std=c++1z)

# If not specified, the default build type is "release"
set(CMAKE_BUILD_TYPE Release)

# BYGO package version
set(BYGO_PACKAGE_VERSION_MAJOR 1)
set(BYGO_PACKAGE_VERSION_MINOR 0)
set(BYGO_PACKAGE_VERSION_PATCH 0)
set(BYGO_PACKAGE_VERSION "${BYGO_PACKAGE_VERSION_MAJOR}.${BYGO_PACKAGE_VERSION_MINOR}.${BYGO_PACKAGE_VERSION_PATCH}")

# BYGO's installation path
set(BYGO_INSTALL_PREFIX "/media/koseng/4A7AE1C07AE1A941/lit/code/install")
set(BYGO_MAIN_INSTALL_PATH "${BYGO_INSTALL_PREFIX}/bygo-${BYGO_PACKAGE_VERSION}")
set(BYGO_INCLUDE_PATH "${BYGO_MAIN_INSTALL_PATH}/include")
set(BYGO_HEADER_PATH "${BYGO_INCLUDE_PATH}/bygo")
set(BYGO_LIB_PATH "${BYGO_MAIN_INSTALL_PATH}/lib/bygo")
set(BYGO_CMAKE_CONFIG_INSTALL_PATH "${BYGO_MAIN_INSTALL_PATH}/share/bygo/cmake")

# BYGO Lib
add_subdirectory(include)

# Build tree
set(CONF_INCLUDE_DIR "${CMAKE_SOURCE_DIR}")
set(CONF_TARGETS_DIR "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/Export/_${BYGO_CMAKE_CONFIG_INSTALL_PATH}")
configure_file(bygo-config.cmake.in "${CMAKE_BINARY_DIR}/bygo-config.cmake" @ONLY)

# Install tree
set(CONF_INCLUDE_DIR "${BYGO_INCLUDE_PATH}")
set(CONF_TARGETS_DIR "${BYGO_CMAKE_CONFIG_INSTALL_PATH}")
configure_file(bygo-config.cmake.in "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/bygo-config.cmake" @ONLY)

install(FILES "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/bygo-config.cmake" DESTINATION "${BYGO_CMAKE_CONFIG_INSTALL_PATH}")
install(EXPORT bygo_export DESTINATION ${BYGO_CMAKE_CONFIG_INSTALL_PATH}
    FILE bygo-targets.cmake
)

# Tests
# add_subdirectory(tests)

# Examples
add_subdirectory(examples)