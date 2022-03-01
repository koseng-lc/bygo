cmake_minimum_required(VERSION 2.8.3)
project(bygo_tests)

find_package(GTest REQUIRED)

add_executable(basic_elem bygo/basic/basic_elem)
target_link_libraries(basic_elem
    ${GTEST_LIBRARIES}
)