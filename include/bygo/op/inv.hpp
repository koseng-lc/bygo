#ifndef BYGO_OP_INV_HPP
#define BYGO_OP_INV_HPP

#include <bygo/op/rref.hpp>

namespace bygo::op{

namespace impl{


    template <typename in_t, typename out_t>
    constexpr auto inv(in_t&& in, out_t&& out){
        out = ::bygo::op::rref(std::forward<in_t>(in));
    }
}

template <typename in_t>
constexpr auto inv(in_t&& in){
    using out_type = util::remove_cvref_t<in_t>;

    out_type res(in);
    
    impl::inv(std::forward<in_t>(in), res);

    return res;
}

}

#endif