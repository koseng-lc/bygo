#ifndef BYGO_OP_STACK_HPP
#define BYGO_OP_STACK_HPP

#include <bygo/basic/basic_elem.hpp>
#include <bygo/prop/shape.hpp>

namespace bygo::op{

namespace impl{

}

template <std::size_t axis, typename in_t, typename op_t>
constexpr auto stack(in_t&& in, op_t&& op){
    using in_type = util::remove_cvref_t<in_t>;
    using out_shape = aux::insert_axis_t<axis, 2, typename in_type::shape_type>;
    using out_type = basic_elem<typename in_type::scalar_type, out_shape>;

    out_type res;

    return res;
}

}

#endif