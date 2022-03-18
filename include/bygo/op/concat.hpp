#ifndef BYGO_OP_CONCAT_HPP
#define BYGO_OP_CONCAT_HPP

#include <bygo/prop/shape.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

namespace impl{

}

template <std::size_t axis, typename in_t, typename op_t>
constexpr auto concat(in_t&& in, op_t&& op){
    using in_type = util::remove_cvref_t<in_t>;
    using op_type = util::remove_cvref_t<op_t>;
    using out_type = aux::add_nth_shape_t<axis, aux::nth_shape_dim_v<typename op_type::shape_type, axis+1>, typename in_type::shape_type>;

    out_type res;

    return res;
}

}

#endif