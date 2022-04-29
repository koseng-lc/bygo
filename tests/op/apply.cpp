#include <bygo/bygo.hpp>

#include <gtest/gtest.h>

struct apply_op_test_t{
    template <typename in_t, typename op_t>
    constexpr auto operator()(in_t&& in, op_t&& op) -> std::common_type_t<in_t, op_t>{
        return in * op + op;
    }
}

TEST(apply_test, op){
    // EXPECT_TRUE();
}

int main(int arc, char** argv){

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}