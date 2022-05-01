#ifndef BYGO_RANDOM_RANDOM_HPP
#define BYGO_RANDOM_RANDOM_HPP

#include <random>

#include <bygo/basic/basic_elem.hpp>

namespace bygo::random{

template <typename shape_t, typename scalar_t = util::default_scalar_t>
constexpr auto gen(){
    basic_elem<shape_t, scalar_t> res{};

    return res;
}

}

#endif