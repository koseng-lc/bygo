#ifndef BYGO_OP_INSERT_HPP
#define BYGO_OP_INSERT_HPP

#include <bygo/basic/basic_elem.hpp>
#include <bygo/op/assign.hpp>
#include <bygo/prop/shape.hpp>
#include <bygo/aux/aux.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

namespace impl{

    /**
     *  @brief Shift the input index after targeted index
     */ 
    template <typename tup_t, std::size_t ...I>
    constexpr auto _after_idx(tup_t&& tup, std::index_sequence<I...>){
        return std::make_tuple((std::get<I>(tup) - (std::size_t)(I == (sizeof...(I)-1)))...);
    }

    template <typename ...Ts>
    constexpr auto after_idx(Ts... ts){
        using Is = std::make_index_sequence<sizeof...(Ts)>;
        return _after_idx(std::forward_as_tuple(ts...), Is{});
    }

    template <std::size_t index, std::size_t axis, std::size_t idx
        , typename shape_t, typename in_t, typename op_t, typename out_t
        , typename op_axes_t, std::size_t ...I
        , std::size_t ...J, typename ...axes_t>
    constexpr auto _insert(in_t&& in, op_t&& op, out_t&& out
        , op_axes_t op_axes, std::index_sequence<I...>
        , std::index_sequence<J...>, axes_t ...axes){

        if constexpr(sizeof...(axes_t)-1 == axis){
            if constexpr(idx < index){
                out = ::bygo::op::assign(std::forward<out_t>(out), std::forward<in_t>(in), std::make_tuple(axes...), std::make_tuple(axes...));
            }else if (idx > index){
                out = ::bygo::op::assign(std::forward<out_t>(out), std::forward<in_t>(in), std::make_tuple(axes...), after_idx(axes...));
            }else{
                out = ::bygo::op::assign(std::forward<out_t>(out), std::forward<op_t>(op), std::make_tuple(axes...), std::make_tuple(std::get<I>(op_axes)...));
            }
        }else{
            using Js = std::make_index_sequence<shape_t::dim>;
            (_insert<index, axis, J, typename shape_t::res_shape>(std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out)
                , op_axes, std::index_sequence<I...>{}
                , Js{}, axes..., J), ...);
        }
    }

    template <std::size_t index, std::size_t axis, typename shape_t
        , typename in_t, typename op_t, typename out_t
        , typename op_axes_t, std::size_t ...I
        , std::size_t ...J>
    constexpr auto insert(in_t&& in, op_t&& op, out_t&& out
        , op_axes_t op_axes, std::index_sequence<I...>
        , std::index_sequence<J...>){

        using Js = std::make_index_sequence<shape_t::dim>;

        (_insert<index, axis, J, typename shape_t::res_shape>(std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out)
            , op_axes, std::index_sequence<I...>{} 
            , Js{}, J), ...);
    }
}

template <std::size_t index, std::size_t axis, typename in_t, typename op_t, typename ...axes_t>
constexpr auto insert(in_t&& in, op_t&& op, std::tuple<axes_t...> axes){

    using in_type = util::remove_cvref_t<in_t>;
    using in_shape = typename in_type::shape_type;

    using out_shape = aux::add_nth_shape_t<axis, 1, in_shape>;
    using out_type = basic_elem<typename in_type::scalar_type, out_shape>;

    using res_shape = aux::nth_shape_t<in_shape, axis+1>;
    using op_shape = typename util::remove_cvref_t<op_t>::shape_type;

    // static_assert(aux::is_shape_equal_v<res_shape, op_shape>, "[insert] The target shape is not compatible.");

    out_type res{};
    impl::insert<index, axis, typename out_shape::res_shape>(std::forward<in_t>(in), std::forward<op_t>(op), res
        , axes, std::make_index_sequence<sizeof...(axes_t)>{}
        , std::make_index_sequence<out_shape::dim>{});

    return res;
}

}

#endif