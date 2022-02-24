#ifndef BYGO_OP_ADD_HPP
#define BYGO_OP_ADD_HPP

#include <bygo/prop/shape.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

template <typename A, typename B, typename C, typename shape_t = typename util::remove_cvref_t<A>::shape_type, typename Is = std::make_index_sequence<shape_t::dim>>
constexpr auto add(A&& a, B&& b, C&& c);

namespace impl{
    template <typename A, typename B, typename C, typename shape_t, std::size_t... I>
    constexpr auto add_impl(A&& a, B&& b, C&& c, std::index_sequence<I...>){
        (add<decltype(std::forward<decltype(a[I])>(a[I])), decltype(std::forward<decltype(b[I])>(b[I])), decltype(std::forward<decltype(c[I])>(c[I])), typename shape_t::res_shape>
            (std::forward<decltype(a[I])>(a[I]), std::forward<decltype(b[I])>(b[I]), std::forward<decltype(c[I])>(c[I])), ...);
    }
}

template <typename A, typename B, typename C, typename shape_t = typename util::remove_cvref_t<A>::shape_type, typename Is = std::make_index_sequence<shape_t::dim>>
constexpr auto add(A&& a, B&& b, C&& c){
    if constexpr(helper::is_scalar_v<util::remove_cvref_t<A>>){
        c = a + b;
    }else{
        impl::add_impl<decltype(std::forward<decltype(a)>(a)), decltype(std::forward<decltype(b)>(b)), decltype(std::forward<decltype(c)>(c)), shape_t>
            (std::forward<decltype(a)>(a), std::forward<decltype(b)>(b), std::forward<decltype(c)>(c), Is{});
    }
}

} // namespace bygo::op

#endif