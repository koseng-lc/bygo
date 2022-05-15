#ifndef BYGO_RANDOM_RANDOM_HPP
#define BYGO_RANDOM_RANDOM_HPP

#include <random>

#include <bygo/basic/entity.hpp>

namespace bygo::random{

namespace impl{

    // Provided to seed the random engine
    static std::random_device rd;
    // PRNG
    static std::mt19937 mt_gen(rd());
    // Distribution
    static std::uniform_int_distribution<> uni_int_distrib(-9, 9);

    struct _rand_gen{
        template <typename ...args_t>
        auto operator()(args_t&& ...args){
            return uni_int_distrib(mt_gen);
        }
    };
} // namespace impl

/**
 * @note Not yet involve the scalar type
 */ 
template <typename shape_t, typename scalar_t = util::default_scalar_t>
auto gen(){

    basic::entity<shape_t, scalar_t> res{};
    bygo::op::apply(impl::_rand_gen(), res);

    return res;
}

} // namespace bygo

#endif