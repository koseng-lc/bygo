#ifndef BYGO_OP_APPLY_HPP
#define BYGO_OP_APPLY_HPP

#include <bygo/util/util.hpp>
#include <bygo/aux/aux.hpp>

namespace bygo::op{

namespace impl{
    template <typename shape_t, typename fn_t, typename in_t, typename op_t, typename out_t, std::size_t... I>
    constexpr void _apply(fn_t&& fn, in_t&& in, op_t&& op, out_t&& out, std::index_sequence<I...>, std::size_t idx = 0){
        if constexpr(aux::is_scalar_v<util::remove_cvref_t<out_t>>){
            out = fn(in, op);
        }else{
            using Is = std::make_index_sequence<shape_t::dim>;
            if constexpr(aux::is_scalar_v<util::remove_cvref_t<op_t>>){
                (_apply<typename shape_t::res_shape>
                    (std::forward<fn_t>(fn), std::forward<decltype(in[I])>(in[I]), std::forward<op_t>(op), std::forward<decltype(out[I])>(out[I]), Is{}), ...);
            }else{
                (_apply<typename shape_t::res_shape>
                    (std::forward<fn_t>(fn), std::forward<decltype(in[I])>(in[I]), std::forward<decltype(op[I])>(op[I]), std::forward<decltype(out[I])>(out[I]), Is{}), ...);
            }
        }
    }

    template <typename shape_t, typename fn_t, typename in_t, typename op_t, typename out_t, std::size_t... I>
    constexpr auto apply(fn_t&& fn, in_t&& in, op_t&& op, out_t&& out, std::index_sequence<I...>){
        using Is = std::make_index_sequence<shape_t::dim>;

        if constexpr(aux::is_scalar_v<util::remove_cvref_t<out_t>>){
            (_apply<typename shape_t::res_shape>
                    (std::forward<fn_t>(fn), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out), Is{}, I), ...);
        }else{
            using Is = std::make_index_sequence<shape_t::dim>;
            if constexpr(aux::is_scalar_v<util::remove_cvref_t<op_t>>){
                (_apply<typename shape_t::res_shape>
                    (std::forward<fn_t>(fn), std::forward<decltype(in[I])>(in[I]), std::forward<op_t>(op), std::forward<decltype(out[I])>(out[I]), Is{}), ...);
            }else{
                (_apply<typename shape_t::res_shape>
                    (std::forward<fn_t>(fn), std::forward<decltype(in[I])>(in[I]), std::forward<decltype(op[I])>(op[I]), std::forward<decltype(out[I])>(out[I]), Is{}), ...);
            }
        }
    }
}

template <typename fn_t, typename in_t, typename op_t, typename out_t, typename shape_t = typename util::remove_cvref_t<out_t>::shape_type>
constexpr auto apply(fn_t&& fn, in_t&& in, op_t&& op, out_t&& out){
    using Is = std::make_index_sequence<shape_t::dim>;

    impl::apply<typename shape_t::res_shape>
        (std::forward<fn_t>(fn), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out), Is{});
}

}

#endif