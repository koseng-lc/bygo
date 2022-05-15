cmake_minimum_required(VERSION 2.8.3)
project(bygo_tests)

find_package(GTest REQUIRED)

add_executable(basic_entity_test bygo/basic/entity.cpp)
target_link_libraries(basic_entity_test
    bygo_lib
    ${GTEST_LIBRARIES}
    pthread
)

add_executable(op_apply_test bygo/op/apply.cpp)
target_link_libraries(op_apply_test
    bygo_lib
    ${GTEST_LIBRARIES}
    pthread
)