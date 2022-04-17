#ifndef BYGO_OP_APPLY_HPP
#define BYGO_OP_APPLY_HPP

#include <bygo/util/util.hpp>
#include <bygo/aux/aux.hpp>

namespace bygo::op{

namespace impl{
    /**
     * @note If we use the universal reference on op_t and in_t, the deduced type include const which cause implicit conversion (?)
     */
    template <typename shape_t, typename fn_t, typename in_t, typename op_t, typename out_t
        , std::size_t... I, typename axes1_t, typename axes2_t, std::size_t... J, std::size_t... K, typename ...axes3_t>
    constexpr void _apply(fn_t&& fn, in_t in, op_t op, out_t&& out
        , std::index_sequence<I...>, axes1_t axes1, axes2_t axes2, std::index_sequence<J...>, std::index_sequence<K...>, axes3_t... axes3){
        if constexpr(aux::is_scalar_v<util::remove_cvref_t<decltype(out(std::get<J>(axes1)..., axes3...))>>){
            if constexpr(aux::is_scalar_v<util::remove_cvref_t<op_t>>){
                out(std::get<J>(axes1)..., axes3...) = fn(in(std::get<J>(axes1)..., axes3...), op);
            }else{
                out(std::get<J>(axes1)..., axes3...) = fn(in(std::get<J>(axes1)..., axes3...), op(std::get<K>(axes2)..., axes3...));
            }
            
        }else{
            using Is = std::make_index_sequence<shape_t::dim>;

            (_apply<typename shape_t::res_shape>
                (std::forward<fn_t>(fn), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out)
                , Is{}, axes1, axes2, std::index_sequence<J...>{}, std::index_sequence<K...>{}, axes3..., I), ...);
        }
    }

    template <typename shape_t, typename fn_t, typename in_t, typename op_t, typename out_t, typename axes1_t, typename axes2_t, std::size_t... I, std::size_t... J>
    constexpr void apply(fn_t&& fn, in_t&& in, op_t&& op, out_t&& out, axes1_t axes1, axes2_t axes2, std::index_sequence<I...>, std::index_sequence<J...>){
        using Is = std::make_index_sequence<shape_t::dim>;

        _apply<typename shape_t::res_shape>
            (std::forward<fn_t>(fn), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out)
            , Is{}, axes1, axes2, std::index_sequence<I...>{}, std::index_sequence<J...>{});
    }
}

template <typename fn_t, typename in_t, typename op_t, typename out_t, typename ...axes1_t, typename ...axes2_t>
constexpr auto apply(fn_t&& fn, in_t&& in, op_t&& op, out_t&& out, std::tuple<axes1_t...> axes1, std::tuple<axes2_t...> axes2){

    using out_shape = typename util::remove_cvref_t<out_t>::shape_type;
    using target_out_shape = aux::nth_shape_t<out_shape, sizeof...(axes1_t) + 1>;
    using Is = std::make_index_sequence<sizeof...(axes1_t)>;

    using op_shape = typename util::remove_cvref_t<op_t>::shape_type;
    using target_op_shape = aux::nth_shape_t<op_shape, sizeof...(axes2_t) + 1>;
    using Js = std::make_index_sequence<sizeof...(axes2_t)>;

    // static_assert(aux::is_shape_equal_v<target_out_shape, target_op_shape>, "[apply] The targeted shape must be equal!");

    impl::apply<target_out_shape>
        (std::forward<fn_t>(fn), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out), axes1, axes2, Is{}, Js{});
}


template <typename fn_t, typename in_t, typename op_t, typename out_t>
constexpr auto apply(fn_t&& fn, in_t&& in, op_t&& op, out_t&& out){

    using out_shape = typename util::remove_cvref_t<out_t>::shape_type;
    using Is = std::make_index_sequence<out_shape::dim>;

    impl::_apply<typename out_shape::res_shape>
        (std::forward<fn_t>(fn), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out), Is{});
}

}

#endif