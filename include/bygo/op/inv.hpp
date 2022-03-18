#ifndef BYGO_OP_INV_HPP
#define BYGO_OP_INV_HPP

#include <bygo/ctype/eye.hpp>
#include <bygo/op/rref.hpp>
#include <bygo/op/concat.hpp>

namespace bygo::op{

namespace impl{

    template <typename in_t, typename op_t, std::size_t ...I, typename row_t>
    constexpr auto _get_sub(in_t&& in, op_t&& op, std::index_sequence<I...>, row_t row){
        using in_type = util::remove_cvref_t<in_t>;
        constexpr auto ncols{sizeof...(I)};
        ((in(row, I) = op(row, I + ncols)), ...);
    }

    template <typename in_t, typename op_t, std::size_t ...I>
    constexpr auto get_sub(in_t&& in, op_t&& op, std::index_sequence<I...>){
        using Is = std::make_index_sequence<aux::nth_shape_dim_v<typename util::remove_cvref_t<in_t>::shape_type, 2>>;
        std::cout << aux::nth_shape_dim_v<typename util::remove_cvref_t<in_t>::shape_type, 2> << std::endl;
        (_get_sub(std::forward<in_t>(in), std::forward<op_t>(op), Is{}, I), ...);
    }

    template <typename in_t, typename out_t>
    constexpr auto inv(in_t&& in, out_t&& out){
        auto I(::bygo::ctype::eye(in));
        auto aug(::bygo::op::concat<1>(in, I));
        auto rref_res(::bygo::op::rref(std::forward<decltype(aug)>(aug)));
        using rref_t = decltype(rref_res);

        using Is = std::make_index_sequence<aux::nth_shape_dim_v<typename util::remove_cvref_t<out_t>::shape_type, 1>>;
        std::cout << aux::nth_shape_dim_v<typename util::remove_cvref_t<out_t>::shape_type, 1> << std::endl;
        get_sub(std::forward<out_t>(out), std::forward<rref_t>(rref_res), Is{});
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