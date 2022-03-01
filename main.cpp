#include <iostream>

#include <bygo.hpp>

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
        t(1,0,1,0) = 19;
        t2(1,0,1,0) = 11;
        // Print element-(0,1,0,1)
        std::cout << t[0][1][0][1] << std::endl;
        // Print element-(0,1,0,1) in reverse order
        std::cout << t(1,0,1,0) << std::endl;

        std::cout << tensor_basic2_t::nelem << std::endl;
        auto res = t + t2;
        bygo::op::fill(t, -1.);
        // res = t + t2;
        std::cout << "Result1: " << res[0][1][0][1] << std::endl;
        std::cout << "Result2: " << t[0][1][0][1] << std::endl;
    }

    using matrix_t = bygo::matrix<double, 3,2>;
    {
        // matrix_t m{{
        //     {1,2},
        //     {3,4},
        //     {5,6}
        // }};
    }

    return 0;
}