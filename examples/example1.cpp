#include <iostream>
#include <memory>
#include <cxxabi.h>

#include <bygo/bygo.hpp>

int main(int argc, [[maybe_unused]] char** argv){

    using tensor_stl_t = bygo::basic_elem<bygo::shape<3,2,4,2>, double, true>;
    using tensor_basic_t = bygo::basic_elem<bygo::shape<3,2,4,2>>;
    using tensor_basic2_t = bygo::basic_elem<bygo::shape<2,4,2>>;
    using tensor_basic3_t = bygo::basic_elem<bygo::shape<4,2>>;

    {
        // Initialize through the nested initializer
        constexpr tensor_stl_t t{{{
            {{
                {{{1,2},{3,4},{5,6},{7,8}}},
                {{{3,4},{1,2},{7,8},{5,6}}}
            }},
            {{
                {{{5,6},{1,2},{7,8},{3,4}}},
                {{{7,8},{3,4},{1,2},{5,6}}}
            }},
            {{
                {{{1,2},{5,6},{3,4},{7,8}}},
                {{{3,4},{1,2},{5,6},{7,8}}}
            }}
        }}};

        // Initialize using brace elision
        constexpr tensor_stl_t t2{
                1,2, 3,4, 5,6, 7,8,
                3,4, 1,2, 7,8, 5,6
            ,
                5,6, 1,2, 7,8, 3,4,
                7,8, 3,4, 1,2, 5,6
            ,
                1,2, 5,6, 3,4, 7,8,
                3,4, 1,2, 5,6, 7,8
        };

        // // Set element-(1,0,1,0)
        // t(0,1,0,1) = 99;
        // // Print element-(1,0,1,0)
        // std::cout << t[1][0][1][0] << std::endl;
        // // Print element-(1,0,1,0) in reverse order
        // std::cout << t(0,1,0,1) << std::endl;
        // std::cout << tensor_stl_t::nelem << std::endl;
    }

    {
        constexpr tensor_basic_t t{{
            {
                {{1,2},{3,4},{5,6},{7,8}},
                {{3,4},{1,2},{7,8},{5,6}}
            },
            {
                {{5,6},{1,2},{7,8},{3,4}},
                {{7,8},{3,4},{1,2},{5,6}}
            },
            {
                {{1,2},{5,6},{3,4},{7,8}},
                {{3,4},{1,2},{5,6},{7,8}}
            }
        }};

        constexpr tensor_basic_t t2{{
            {
                {{1,2},{3,4},{5,6},{7,8}},
                {{3,4},{1,2},{7,8},{5,6}}
            },
            {
                {{5,6},{1,2},{7,8},{3,4}},
                {{7,8},{3,4},{1,2},{5,6}}
            },
            {
                {{1,2},{5,6},{3,4},{7,8}},
                {{3,4},{1,2},{5,6},{7,8}}
            }
        }};

        constexpr tensor_basic2_t t3{{
            {
                {9,1},{3,4},{5,6},{7,8}
            },
            {
                {3,4},{1,2},{7,8},{5,6}
            }
        }};

        constexpr tensor_basic3_t t4{{
            {1,2},
            {3,4},
            {5,6},
            {7,8}
        }};
        
        // Set element-(0,1,0,1)
        // t(1,0,1,0) = 19;
        // t(2,1,3,1) = 19;
        // t2(1,0,1,0) = 11;

        // Print element-(0,1,0,1)
        std::cout << "t(2,1,3,1): " << t[2][1][3][1] << std::endl;
        std::cout << "t(2,1,3,1): " << t(2,1,3,1) << std::endl;

        // bygo::op::fill(t, -1.);
        // std::cout << "Result1: " << res[0][1][0][1] << std::endl;
        // std::cout << "Result2: " << t[0][1][0][1] << std::endl;

        constexpr auto t_add(bygo::op::add(t, t2, bygo::ax(0,1,0), bygo::ax(1,0,0)));
        std::cout << "Add:" << std::endl;
        bygo::util::print(t_add);

        constexpr auto t_subtract(bygo::op::subtract(t, t2, bygo::ax(0,1,0), bygo::ax(1,0,0)));
        std::cout << "Subtract:" << std::endl;
        bygo::util::print(t_subtract);

        constexpr auto t_assign(bygo::op::assign(t, t2, bygo::ax(0,0), bygo::ax(1,0)));
        std::cout << "Assign:" << std::endl;
        bygo::util::print(t_assign);

        constexpr auto t_ins(bygo::op::insert(t2, t, BYGO_K(1), BYGO_K(1), bygo::ax(0,1)));
        std::cout << "Insert:" << std::endl;
        bygo::util::print(t_ins);

        constexpr auto t_stack(bygo::op::stack(t, t2, BYGO_K(1)));
        std::cout << "Stack:" << std::endl;
        bygo::util::print(t_stack);

        constexpr auto t_concat(bygo::op::concat(t, t2, BYGO_K(3)));
        std::cout << "Concat:" << std::endl;
        bygo::util::print(t_concat);
        
        constexpr auto t_slice(bygo::op::slice(t, BYGO_IVAL(1,1), BYGO_IVAL(0,0), BYGO_IVAL_ALL()));
        std::cout << "Slice:" << std::endl;
        bygo::util::print(t_slice);

        // std::cout << "Is Equal: " << bygo::aux::is_equal(t, t2) << std::endl;

        auto t_random(bygo::random::gen<bygo::shape<3,4,2,5>>());
        std::cout << "Random:" << std::endl;
        bygo::util::print(t_random);

        // bygo::util::print_seq(bygo::aux::shape_dim_t<decltype(t_slice)::shape_type>{});
    }

    using matrix_t = bygo::matrix<3, 2>;
    using matrix_rref_t = bygo::matrix<3, 4>;
    using matrix_sqr_t = bygo::matrix<3, 3>;
    using vec_t = bygo::basic_elem<bygo::shape<2>>;
    {
        constexpr matrix_t m({{
            {1,2},
            {3,4},
            {5,6}
        }});

        matrix_t m2({{
            {2,9},
            {7,5},
            {1,3}
        }});

        // m(0,1) = 99;
        std::cout << "m(0,0): " << m(0,1) << std::endl;
        std::cout << "Matrix Cols. Size: " << matrix_t::ncols << std::endl;
        std::cout << "Matrix Rows. Size: " << matrix_t::nrows << std::endl;

        bygo::util::print(m);

        // auto m2_t(bygo::op::transpose(m));
        // constexpr auto m_t(bygo::op::transpose(m));
        // auto m_t_cols(decltype(m_t)::ncols);
        // auto m_t_rows(decltype(m_t)::nrows);
        // std::cout << "Matrix_T Cols. Size: " << m_t_cols << std::endl;
        // std::cout << "Matrix_T Rows. Size: " << m_t_rows << std::endl;

        // bygo::util::print(m_t);

        // constexpr auto m_sym(bygo::op::dot(m, m_t));
        // std::cout << "======" << std::endl;
        // bygo::util::print(m_sym);

        // constexpr auto x = IS_CONSTEXPR(bygo::op::transpose(m));
        // constexpr auto y = IS_CONSTEXPR(bygo::op::transpose(m2));

        // std::cerr << "Compile-time: " << x << std::endl;
        // std::cerr << "Compile-time: " << y << std::endl;

        // std::cout << "======M2" << std::endl;
        // bygo::util::print(m2);

        // auto m3(m);
        // // m3(0) = m2(1);

        // matrix_rref_t m_rref({{
        //     {1,2,-1,-4},
        //     {2,3,-1,-11},
        //     {-2,0,-3,22}
        // }});

        constexpr matrix_sqr_t m_inv_target({{
            {1,2,-1},
            {2,3,-1},
            {-2,0,3}
        }});

        // constexpr auto m_swapped(bygo::op::swap_elem(m, std::make_tuple(1), std::make_tuple(2)));
        // bygo::util::print(m_swapped);

        constexpr auto m_inv(bygo::op::inv(m_inv_target));
        std::cout << "M_inv" << std::endl;
        bygo::util::print(m_inv);
        
        // vec_t v{{
        //     1,2
        // }};
        // auto m_ins(bygo::op::insert(m, v, std::make_tuple(1)));

        // bygo::util::print_seq(bygo::aux::shape_dim_t<decltype(m_ins)::shape_type>{});
    }

    return 0;
}