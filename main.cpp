#include <iostream>
#include <memory>
#include <cxxabi.h>

#include <bygo.hpp>
#include <bygo/prop/ax.hpp>

template <typename T>
constexpr void test_helper(T&&){}

#define IS_CONSTEXPR(...) noexcept(test_helper(__VA_ARGS__))

template <std::size_t ...I>
constexpr auto check(std::index_sequence<I...>){
    (std::cout << I << " ", ...);
    std::cout << std::endl;
}

struct op_apply{
    template <typename in_t, typename op_t>
    constexpr auto operator()(in_t&& in, op_t&& op){
        return in + op;
    }
};

int main(int argc, char** argv){

    using tensor_stl_t = bygo::basic_elem<double, bygo::shape<3,2,4,2>, true>;
    using tensor2_t = bygo::basic_elem<double, bygo::shape<3,2,3>>;
    using tensor_basic2_t = bygo::basic_elem<double, bygo::shape<3,2,4,2>>;
    using tensor_basic3_t = bygo::basic_elem<double, bygo::shape<2,4,2>>;
    using tensor_basic4_t = bygo::basic_elem<double, bygo::shape<4,2>>;

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
        constexpr tensor_basic2_t t{{
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

        constexpr tensor_basic2_t t2{{
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

        constexpr tensor_basic3_t t3{{
            {
                {9,1},{3,4},{5,6},{7,8}
            },
            {
                {3,4},{1,2},{7,8},{5,6}
            }
        }};

        constexpr tensor_basic4_t t4{{
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

        constexpr auto t_add(bygo::op::add(t, t2, std::make_tuple(0,1,0), std::make_tuple(1,0,0)));
        std::cout << "Add:" << std::endl;
        bygo::util::print(t_add);

        constexpr auto t_assign(bygo::op::assign(t, t2, std::make_tuple(0,0), std::make_tuple(1,0)));
        std::cout << "Assign:" << std::endl;
        bygo::util::print(t_assign);

        constexpr auto t_ins(bygo::op::insert<1, 1>(t2, t, std::make_tuple(0,1)));
        std::cout << "Insert:" << std::endl;
        bygo::util::print(t_ins);

        constexpr auto t_stack(bygo::op::stack<1>(t, t2));
        std::cout << "Stack:" << std::endl;
        bygo::util::print(t_stack);

        constexpr auto t_concat(bygo::op::concat<3>(t, t2));
        std::cout << "Concat:" << std::endl;
        bygo::util::print(t_concat);

        // check(bygo::aux::shape_dim_t<decltype(t_concat)::shape_type>{});
    }

    using matrix_t = bygo::matrix<double, 3, 2>;
    using matrix_rref_t = bygo::matrix<double, 3, 4>;
    using matrix_sqr_t = bygo::matrix<double, 3, 3>;
    using vec_t = bygo::basic_elem<double, bygo::shape<2>>;
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

        // auto m2_t(bygo::op::transpose(m));
        // constexpr auto m_t(bygo::op::transpose(m));
        // auto m_t_cols(decltype(m_t)::ncols);
        // auto m_t_rows(decltype(m_t)::nrows);
        // std::cout << "Matrix_T Cols. Size: " << m_t_cols << std::endl;
        // std::cout << "Matrix_T Rows. Size: " << m_t_rows << std::endl;

        // bygo::util::print_matrix(m_t);

        // constexpr auto m_sym(bygo::op::dot(m, m_t));
        // std::cout << "======" << std::endl;
        // bygo::util::print_matrix(m_sym);

        // constexpr auto x = IS_CONSTEXPR(bygo::op::transpose(m_t));
        // constexpr auto y = IS_CONSTEXPR(bygo::op::transpose(m2_t));

        // std::cerr << "Compile-time: " << x << std::endl;
        // std::cerr << "Compile-time: " << y << std::endl;

        // std::cout << "======M2" << std::endl;
        // bygo::util::print_matrix(m2);

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

        // // constexpr auto m_swapped(bygo::op::swap_elem(m, std::make_tuple(1), std::make_tuple(2)));
        // // bygo::util::print_matrix(m_swapped);
        // auto m_add(bygo::op::add(m, m2, std::make_tuple(1), std::make_tuple(0)));
        // // std::cout << "====== Add sub:" << std::endl;
        // // bygo::util::print_matrix(m_add);

        constexpr auto m_inv(bygo::op::inv(m_inv_target));
        std::cout << "M_inv" << std::endl;
        bygo::util::print_matrix(m_inv);

        // auto m_concat(bygo::op::concat<1>(m_rref, m2));
        // bygo::util::print(m_concat);

        // vec_t v{{
        //     1,2
        // }};
        // auto m_ins(bygo::op::insert(m, v, std::make_tuple(1)));

        // check(bygo::aux::shape_dim_t<decltype(m_ins)::shape_type>{});
    }

    return 0;
}