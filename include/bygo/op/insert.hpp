#ifndef BYGO_OP_INSERT_HPP
#define BYGO_OP_INSERT_HPP

#include <bygo/basic/basic_elem.hpp>
#include <bygo/op/assign.hpp>
#include <bygo/prop/shape.hpp>
#include <bygo/aux/aux.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

namespace impl{
    template <typename shape_t, typename in_t, typename op_t, typename out_t, typename axes_t, std::size_t ...I>
    constexpr auto insert(in_t&& in, op_t&& op, out_t&& out, axes_t&& axes, std::index_sequence<I...>){
        using res_shape = typename shape_t::res_shape;
        using Is = std::make_index_sequence<res_shape::dim>;
        // if(){
        //     out = ::bygo::op::assign(std::forward<out_t>(out), std::forward<in_t>(in), axes, axes);
        //     out = ::bygo::op::assign(std::forward<out_t>(out), std::forward<op_t>(op), axes);
        //     out = ::bygo::op::assign(std::forward<out_t>(out), std::forward<in_t>(in), axes);
        // }else{
        //     (insert<res_shape>(std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out), Is{}), ...);
        // }
    }
}

template <typename in_t, typename op_t, typename ...axes_t>
constexpr auto insert(in_t&& in, op_t&& op, std::tuple<axes_t...> axes){
    // static_assert()
    using in_type = util::remove_cvref_t<in_t>;
    using in_shape = typename in_type::shape_type;
    using shape_type = aux::add_nth_shape_t<sizeof...(axes_t)-1, 1, in_shape>;
    using out_type = basic_elem<typename in_type::scalar_type, shape_type>;
    using out_shape = typename out_type::shape_type;
    using res_shape = aux::nth_shape_t<in_shape, sizeof...(axes_t)+1>;
    using op_shape = typename util::remove_cvref_t<op_t>::shape_type;

    static_assert(aux::is_shape_equal_v<res_shape, op_shape>, "[insert] The target shape is not compatible.");

    out_type res;
    impl::insert<out_shape>(std::forward<in_t>(in), std::forward<op_t>(op), res, axes, std::make_index_sequence<sizeof...(axes_t)>{});

    return res;
}

}

#endif