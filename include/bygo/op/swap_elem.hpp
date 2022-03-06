#ifndef BYGO_OP_SWAP_ELEM_HPP
#define BYGO_OP_SWAP_ELEM_HPP

#include <bygo/op/assign.hpp>

namespace bygo::op{

namespace impl{
    template <typename in_t, typename out_t, typename loc1_t, typename loc2_t, std::size_t ...I>
    constexpr auto swap_elem(in_t&& in, out_t&& out, loc1_t&& loc1, loc2_t&& loc2, std::index_sequence<I...>){
        out = ::bygo::op::assign(std::forward<out_t>(out), std::forward<in_t>(in), loc1, loc2);
        out = ::bygo::op::assign(std::forward<out_t>(out), std::forward<in_t>(in), loc2, loc1);
    }
}

template <typename in_t, typename ...loc1_t, typename ...loc2_t, typename Is = std::make_index_sequence<sizeof...(loc1_t)>>
constexpr auto swap_elem(in_t&& in, std::tuple<loc1_t...> loc1, std::tuple<loc2_t...> loc2){
    static_assert(sizeof...(loc1_t) == sizeof...(loc2_t), "[swap_elem] Number of axis must be the same.");
    using out_type = util::remove_cvref_t<in_t>;

    auto res(in);
    impl::swap_elem(std::forward<in_t>(in), res, loc1, loc2, Is{});

    return res;
}

} // namespace bygo::op

#endif