cmake_minimum_required(VERSION 2.8.3)
project(bygo_tests)

find_package(GTest REQUIRED)

add_executable(basic_elem_test bygo/basic/basic_elem.cpp)

target_link_libraries(basic_elem_test
    bygo_lib
    ${GTEST_LIBRARIES}
    pthread
)