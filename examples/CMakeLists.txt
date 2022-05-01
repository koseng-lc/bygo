cmake_minimum_required(VERSION 2.8.3)
project(bygo_examples)

add_executable(${PROJECT_NAME} example1.cpp)

target_link_libraries(${PROJECT_NAME}
    bygo_lib
)