#ifndef BYGO_OP_INV_HPP
#define BYGO_OP_INV_HPP

#include <bygo/ctype/eye.hpp>
#include <bygo/op/rref.hpp>
#include <bygo/op/concat.hpp>
#include <bygo/op/slice.hpp>

namespace bygo::op{

namespace impl{

    template <typename in_t, typename out_t>
    constexpr auto inv(in_t&& in, out_t&& out){

        auto iden(::bygo::ctype::eye(std::forward<in_t>(in)));
        auto aug(::bygo::op::concat(std::forward<in_t>(in), iden, BYGO_K(1)));
        auto rref_res(::bygo::op::rref(aug));

        using rref_type = decltype(rref_res);
        using rref_shape = typename rref_type::shape_type;

        constexpr auto rref_cols(rref_shape::res_shape::dim);

        out = bygo::op::slice(rref_res, BYGO_IVAL_ALL(), BYGO_IVAL(rref_cols*.5, rref_cols-1));
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