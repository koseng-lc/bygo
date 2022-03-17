#ifndef BYGO_OP_STACK_HPP
#define BYGO_OP_STACK_HPP

#include <bygo/basic/basic_elem.hpp>
#include <bygo/prop/shape.hpp>

namespace bygo::op{

namespace impl{

    template <typename in_t, typename op_t, typename out_t, std::size_t ...I>
    constexpr auto stack(in_t&& in, op_t&& op, out_t&& out, std::index_sequence<I...>){
        ((out(std::integral_constant<std::size_t, I>{}) = in(std::integral_constant<std::size_t, I>{})), ...);
        ((out(std::integral_constant<std::size_t, I + sizeof...(I)>{}) = op(std::integral_constant<std::size_t, I>{})), ...);
    }
}

template <std::size_t axis, typename in_t, typename op_t>
constexpr auto stack(in_t&& in, op_t&& op){
    using in_type = util::remove_cvref_t<in_t>;
    using out_shape = aux::insert_axis_t<axis, 2, typename in_type::shape_type>;
    using out_type = basic_elem<typename in_type::scalar_type, out_shape>;
    using Is = std::make_index_sequence<in_type::nelem>;

    out_type res;
    impl::stack(std::forward<in_t>(in), std::forward<op_t>(op), res, Is{});

    return res;
}

}

#endif