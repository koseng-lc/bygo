#ifndef BYGO_FILL_HPP
#define BYGO_FILL_HPP

#include <bygo/prop/shape.hpp>
#include <bygo/util/util.hpp>

namespace bygo{

template <typename basic_elem_t, typename elem_t,
            typename shape_t = typename util::remove_cvref_t<basic_elem_t>::shape_type, typename Is = std::make_index_sequence<shape_t::dim>>
constexpr auto fill(basic_elem_t&& a, elem_t&& _val);

namespace impl{
    template <typename basic_elem_t, typename elem_t, typename shape_t, std::size_t... I>
    constexpr auto fill_impl(basic_elem_t&& a, elem_t&& _val, std::index_sequence<I...>){
        (fill<decltype(std::forward<decltype(a[I])>(a[I])), decltype(std::forward<decltype(_val)>(_val)), typename shape_t::res_shape>
            (std::forward<decltype(a[I])>(a[I]), std::forward<decltype(_val)>(_val)), ...);
    }
}

template <typename basic_elem_t, typename elem_t,
            typename shape_t = typename util::remove_cvref_t<basic_elem_t>::shape_type, typename Is = std::make_index_sequence<shape_t::dim>>
constexpr auto fill(basic_elem_t&& a, elem_t&& _val){
    // static_assert(helper::is_shape_equal_v<A::shape, B::shape>, "[basic_elem] Shape must be the same!");
    // std::cout << shape_t::dim << std::endl;
    if constexpr(helper::is_scalar_v<util::remove_cvref_t<basic_elem_t>>){
        a = _val;
    }else{
        impl::fill_impl<decltype(std::forward<decltype(a)>(a)), decltype(std::forward<decltype(_val)>(_val)), shape_t>
            (std::forward<decltype(a)>(a), std::forward<decltype(_val)>(_val), Is{});
    }
}

} // namespace bygo

#endif