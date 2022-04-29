cmake_minimum_required(VERSION 2.8.3)
project(bygo_example)

add_compile_options(-std=c++1z)

include_directories(
    include
)

add_library(bygo_lib
    include/bygo/bygo.hpp
)
set_target_properties(bygo_lib PROPERTIES LINKER_LANGUAGE CXX)

add_library(bygo_shape
    include/bygo/prop/shape.hpp
)
set_target_properties(bygo_shape PROPERTIES LINKER_LANGUAGE CXX)

add_library(bygo_elem
    include/bygo/basic/basic_elem.hpp
)
set_target_properties(bygo_elem PROPERTIES LINKER_LANGUAGE CXX)

add_library(bygo_aux
    include/bygo/aux/aux.hpp
)
set_target_properties(bygo_aux PROPERTIES LINKER_LANGUAGE CXX)

add_library(bygo_matrix
    include/bygo/basic/matrix.hpp
)
set_target_properties(bygo_matrix PROPERTIES LINKER_LANGUAGE CXX)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    bygo_lib
    bygo_shape
    bygo_elem
)

# TEST

add_subdirectory(tests)