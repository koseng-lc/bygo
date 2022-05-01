#include <bygo/bygo.hpp>

#include <gtest/gtest.h>

struct apply_op_test_t{
    template <typename in_t, typename op_t>
    constexpr auto operator()(in_t&& in, op_t&& op) -> std::common_type_t<in_t, op_t>{
        return in * op + op;
    }
};

TEST(apply_test, op){
   bygo::basic_elem<bygo::shape<3,4,2,5>> e1{
        {
            {
                {
                    {9,7,-7,-4,-3},
                    {0,6,2,-9,-1}
                },{
                    {7,3,6,-1,4},
                    {-1,8,7,0,1}
                },{
                    {-7,-8,7,-1,-2},
                    {-2,-4,5,-3,-8}
                },{
                    {-9,0,-5,-5,-6},
                    {-9,-1,0,6,1}
                }
            },{
                {
                    {4,-1,9,1,-2},
                    {6,9,-5,-7,9}
                },{
                    {-1,-6,-4,4,3},
                    {3,9,7,2,-2}
                },{
                    {-3,4,5,-4,-8},
                    {9,6,-6,9,-7}
                },{
                    {8,-2,-7,-4,-7},
                    {-9,4,0,-2,-9}
                }
            },{
                {
                    {-2,8,4,0,9},
                    {-2,5,6,3,-8}
                },{
                    {-3,-4,7,1,-7},
                    {-8,7,6,5,1}
                },{
                    {-6,5,3,-1,-2},
                    {0,-6,-7,2,-7}
                },{
                    {-8,-5,5,-1,0},
                    {0,9,2,5,3}
                }
            }
        }
    };

    decltype(e1) e2{
        {
            {
                {
                    {-4,-5,2,-7,-4},
                    {-1,4,-4,1,9}
                },{
                    {-6,-6,-9,9,4},
                    {2,-3,5,1,9}
                },{
                    {-8,-5,-4,-3,2},
                    {-6,7,6,8,-9}
                },{
                    {0,9,3,7,5},
                    {3,-4,7,3,-3}
                }
            },{
                {
                    {-3,6,3,-4,-6},
                    {7,-9,0,-9,6}
                },{
                    {-9,-8,0,-5,-4},
                    {0,-9,4,-8,-5}
                },{
                    {0,-1,-1,-1,3},
                    {-6,-6,-5,9,9}
                },{
                    {9,1,0,3,-7},
                    {-8,7,-2,5,-8}
                }
            },{
                {
                    {-5,1,3,-6,2},
                    {0,-9,-3,5,5}
                },{
                    {9,-7,-1,-2,-5},
                    {0,-5,7,-6,0}
                },{
                    {6,-8,4,5,1},
                    {5,-2,-9,-8,3}
                },{
                    {8,-5,-7,0,-4},
                    {9,-7,-2,5,-8}
                }
            }
        }
    };

    {
        decltype(e1) e3{};
        bygo::op::apply(apply_op_test_t(), e1, e2, e3);
        
        decltype(e3) res{
            {
                {
                    {
                        {-40,-40,-12,21,8},
                        {-1,28,-12,-8,0}
                    },{
                        {-48,-24,-63,0,20},
                        {0,-27,40,1,18}
                    },{
                        {48,35,-32,0,-2},
                        {6,-21,36,-16,63}
                    },{
                        {0,9,-12,-28,-25},
                        {-24,0,7,21,-6}
                    }
                },{
                    {
                        {-15,0,30,-8,6},
                        {49,-90,0,54,60}
                    },{
                        {0,40,0,-25,-16},
                        {0,-90,32,-24,5}
                    },{
                        {0,-5,-6,3,-21},
                        {-60,-42,25,90,-54}
                    },{
                        {81,-1,0,-9,42},
                        {64,35,-2,-5,64}
                    }
                },{
                    {
                        {5,9,15,-6,20},
                        {0,-54,-21,20,-35}
                    },{
                        {-18,21,-8,-4,30},
                        {0,-40,49,-36,0}
                    },{
                        {-30,-48,16,0,-1},
                        {5,10,54,-24,-18}
                    },{
                        {-56,20,-42,0,-4},
                        {9,-70,-6,30,-32}
                    }
                }
            }
        };

        ASSERT_TRUE(bygo::aux::is_equal(e3, res));
    }
    
    {
        decltype(e1) e3(e1);
        bygo::op::apply(apply_op_test_t(), e1, e2, e3, std::make_tuple(0,1), std::make_tuple(1,0));
        
        decltype(e3) res{
            {
                {
                    {
                        {9,7,-7,-4,-3},
                        {0,6,2,-9,-1}
                    },{
                        {-24,24,21,0,-30},
                        {0,-81,0,-9,12}
                    },{
                        {-7,-8,7,-1,-2},
                        {-2,-4,5,-3,-8}
                    },{
                        {-9,0,-5,-5,-6},
                        {-9,-1,0,6,1}
                    }
                },{
                    {
                        {4,-1,9,1,-2},
                        {6,9,-5,-7,9}
                    },{
                        {-1,-6,-4,4,3},
                        {3,9,7,2,-2}
                    },{
                        {-3,4,5,-4,-8},
                        {9,6,-6,9,-7}
                    },{
                        {8,-2,-7,-4,-7},
                        {-9,4,0,-2,-9}
                    }
                },{
                    {
                        {-2,8,4,0,9},
                        {-2,5,6,3,-8}
                    },{
                        {-3,-4,7,1,-7},
                        {-8,7,6,5,1}
                    },{
                        {-6,5,3,-1,-2},
                        {0,-6,-7,2,-7}
                    },{
                        {-8,-5,5,-1,0},
                        {0,9,2,5,3}
                    }
                }
            }
        };

        ASSERT_TRUE(bygo::aux::is_equal(e3, res));
    }
}

int main(int argc, char** argv){

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}