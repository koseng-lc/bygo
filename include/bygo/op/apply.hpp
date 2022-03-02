#ifndef BYGO_OP_APPLY_HPP
#define BYGO_OP_APPLY_HPP

#include <bygo/util/util.hpp>
#include <bygo/aux/aux.hpp>

namespace bygo::op{

template <typename fn_t, typename in_t, typename op_t, typename out_t, typename shape_t = typename util::remove_cvref_t<out_t>::shape_type, typename Is = std::make_index_sequence<shape_t::dim>>
constexpr auto apply(fn_t&& fn, in_t&& in, op_t&& op, out_t&& out);

namespace impl{
    template <typename fn_t, typename in_t, typename op_t, typename out_t, typename shape_t, std::size_t... I>
    constexpr auto apply(fn_t&& fn, in_t&& in, op_t&& op, out_t&& out, std::index_sequence<I...>){
        // using fn_fwd_t = decltype(std::forward<de cltype(fn)>(fn));
        // using in_fwd_t = decltype(std::forward<decltype(in[I])>(in[I]));
        // using op_fwd_t = decltype(std::forward<decltype(op[I])>(op[I]));
        // using out_fwd_t = decltype(std::forward<decltype(out[I])>(out[I]));
        
        if constexpr(aux::is_scalar_v<util::remove_cvref_t<op_t>>){
            (::bygo::op::apply<decltype(std::forward<fn_t>(fn)),
                decltype(std::forward<decltype(in[I])>(in[I])),
                decltype(std::forward<op_t>(op)),
                decltype(std::forward<decltype(out[I])>(out[I])), typename shape_t::res_shape>
                (std::forward<fn_t>(fn), std::forward<decltype(in[I])>(in[I]), std::forward<op_t>(op), std::forward<decltype(out[I])>(out[I])), ...);
        }else{
            (::bygo::op::apply<decltype(std::forward<fn_t>(fn)),
                decltype(std::forward<decltype(in[I])>(in[I])),
                decltype(std::forward<decltype(op[I])>(op[I])),
                decltype(std::forward<decltype(out[I])>(out[I])), typename shape_t::res_shape>
                (std::forward<fn_t>(fn), std::forward<decltype(in[I])>(in[I]), std::forward<decltype(op[I])>(op[I]), std::forward<decltype(out[I])>(out[I])), ...);
        }
    }
}

template <typename fn_t, typename in_t, typename op_t, typename out_t, typename shape_t = typename util::remove_cvref_t<out_t>::shape_type, typename Is = std::make_index_sequence<shape_t::dim>>
constexpr auto apply(fn_t&& fn, in_t&& in, op_t&& op, out_t&& out){
    if constexpr(aux::is_scalar_v<util::remove_cvref_t<out_t>>){
        out = fn(in, op);
    }else{
        // using fn_fwd_t = decltype(std::forward<decltype(fn)>(fn));
        // using in_fwd_t = decltype(std::forward<decltype(in[I])>(in[I]));
        // using op_fwd_t = decltype(std::forward<decltype(op[I])>(op[I]));
        // using out_fwd_t = decltype(std::forward<decltype(out[I])>(out[I]));

        ::bygo::op::impl::apply<decltype(std::forward<fn_t>(fn)),
            decltype(std::forward<in_t>(in)),
            decltype(std::forward<op_t>(op)),
            decltype(std::forward<out_t>(out)), shape_t>
            (std::forward<fn_t>(fn), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out), Is{});
    }
}

}

#endif