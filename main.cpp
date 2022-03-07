#include <iostream>

#include <bygo.hpp>

template <typename T>
constexpr void test_helper(T&&){}

#define IS_CONSTEXPR(...) noexcept(test_helper(__VA_ARGS__))


int main(int argc, char** argv){
    
    // bygo::basic_elem<double, bygo::shape<3,2>> check{{1,2},{1,2},{1,2}};

    using tensor_stl_t = bygo::basic_elem<double, bygo::shape<3,2,4,2>, true>;
    using tensor2_t = bygo::basic_elem<double, bygo::shape<3,2,3>>;
    using tensor_basic_t = bygo::basic_elem<double, bygo::shape<3,2>>;
    using tensor_basic2_t = bygo::basic_elem<double, bygo::shape<3,2,4,2>>;

    using shape_t = bygo::shape<3,4,5>;

    // std::cout << shape_t::dim << std::endl;
    // std::cout << shape_t::size << std::endl;
    // std::cout << shape_t::res_shape::dim << std::endl;
    // std::cout << shape_t::res_shape::size << std::endl;
    // std::cout << shape_t::res_shape::res_shape::dim << std::endl;
    // std::cout << shape_t::res_shape::res_shape::size << std::endl;

    {
        // tensor_stl_t t{{{
        //     {{
        //         {{{1,2},{3,4},{5,6},{7,8}}},
        //         {{{3,4},{1,2},{7,8},{5,6}}}
        //     }},
        //     {{
        //         {{{5,6},{1,2},{7,8},{3,4}}},
        //         {{{7,8},{3,4},{1,2},{5,6}}}
        //     }},
        //     {{
        //         {{{1,2},{5,6},{3,4},{7,8}}},
        //         {{{3,4},{1,2},{5,6},{7,8}}}
        //     }}
        // }}};
        // // Set element-(1,0,1,0)
        // t(0,1,0,1) = 99;
        // // Print element-(1,0,1,0)
        // std::cout << t[1][0][1][0] << std::endl;
        // // Print element-(1,0,1,0) in reverse order
        // std::cout << t(0,1,0,1) << std::endl;
        // std::cout << tensor_stl_t::nelem << std::endl;
    }

    {
        // tensor_basic_t t{{
        //     {1,2},
        //     {3,4},
        //     {5,6},
        // }};
        // t(1, 2) = 9;
        // // in normal order
        // std::cout << t[2][1] << std::endl;
        // std::cout << t[1] << std::endl;
        // // in reverse order
        // std::cout << t(1,2) << std::endl;

        // std::cout << tensor_basic_t::nelem << std::endl;
    }

    {
        tensor_basic2_t t{{
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

        tensor_basic2_t t2{{
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
        
        // Set element-(0,1,0,1)
        // t(1,0,1,0) = 19;
        t(0,1,0,1) = 19;
        // t2(1,0,1,0) = 11;
        // Print element-(0,1,0,1)
        std::cout << "t(0,1,0,1): " << t[0][1][0][1] << std::endl;
        // Print element-(0,1,0,1) in reverse order
        // std::cout << t(1,0,1,0) << std::endl;
        std::cout << "t(0,1,0,1): " << t(0,1,0,1) << std::endl;
        // std::cout << "CHECK1: " << t.at(1,0,1,0) << std::endl;

        std::cout << tensor_basic2_t::nelem << std::endl;
        auto res = t + t2;
        bygo::op::fill(t, -1.);
        // res = t + t2;
        std::cout << "Result1: " << res[0][1][0][1] << std::endl;
        std::cout << "Result2: " << t[0][1][0][1] << std::endl;
    }

    using matrix_t = bygo::matrix<double, 3, 2>;
    using matrix_rref_t = bygo::matrix<double, 3, 4>;
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

        bygo::util::print_matrix(m);

        auto m2_t(bygo::op::transpose(m));
        constexpr auto m_t(bygo::op::transpose(m));
        auto m_t_cols(decltype(m_t)::ncols);
        auto m_t_rows(decltype(m_t)::nrows);
        std::cout << "Matrix_T Cols. Size: " << m_t_cols << std::endl;
        std::cout << "Matrix_T Rows. Size: " << m_t_rows << std::endl;

        bygo::util::print_matrix(m_t);

        constexpr auto m_sym(bygo::op::dot(m, m_t));
        std::cout << "======" << std::endl;
        bygo::util::print_matrix(m_sym);

        constexpr auto x = IS_CONSTEXPR(bygo::op::transpose(m_t));
        constexpr auto y = IS_CONSTEXPR(bygo::op::transpose(m2_t));

        std::cerr << "Compile-time: " << x << std::endl;
        std::cerr << "Compile-time: " << y << std::endl;

        std::cout << "======M2" << std::endl;
        bygo::util::print_matrix(m2);

        // constexpr auto m_inv(bygo::op::inv(m));
        auto m3(m);
        // m3(0) = m2(1);

        matrix_rref_t m_rref({{
            {1,2,-1,-4},
            {2,3,-1,-11},
            {-2,0,-3,22}
        }});

        constexpr auto m_swapped(bygo::op::swap_elem(m, std::make_tuple(1), std::make_tuple(2)));
        bygo::util::print_matrix(m_swapped);
        auto m_add(bygo::op::add(m, m2, std::make_tuple(1), std::make_tuple(0)));
        std::cout << "====== Add sub:" << std::endl;
        bygo::util::print_matrix(m_add);
        // auto m_inv(bygo::op::inv(m));
        auto m_inv(bygo::op::inv(m_rref));
        std::cout << "======M_inv" << std::endl;
        bygo::util::print_matrix(m_inv);
        auto m_assign(bygo::op::assign(m, m2, std::make_tuple(1)));
        // auto m_assign(bygo::op::assign(m,m2));
        std::cout << "======Assign sub:" << std::endl;
        bygo::util::print_matrix(m_assign);
    }

    return 0;
}