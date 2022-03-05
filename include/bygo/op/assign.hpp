#ifndef BYGO_OP_ASSIGN_HPP
#define BYGO_OP_ASSIGN_HPP

#include <bygo/op/apply.hpp>
#include <bygo/aux/aux.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

// template <typename fn_t, typename in_t, typename op_t, typename out_t, typename shape_t = typename util::remove_cvref_t<out_t>::shape_type, typename Is = std::make_index_sequence<shape_t::dim>>
// constexpr auto apply(fn_t&& fn, in_t&& in, op_t&& op, out_t&& out);

// namespace impl{
//     template <typename fn_t, typename in_t, typename op_t, typename out_t, typename shape_t, std::size_t... I>
//     constexpr auto apply(fn_t&& fn, in_t&& in, op_t&& op, out_t&& out, std::index_sequence<I...>){
        
//         if constexpr(aux::is_scalar_v<util::remove_cvref_t<op_t>>){
//             (::bygo::op::apply<decltype(std::forward<fn_t>(fn)),
//                 decltype(std::forward<decltype(in[I])>(in[I])),
//                 decltype(std::forward<op_t>(op)),
//                 decltype(std::forward<decltype(out[I])>(out[I])), typename shape_t::res_shape>
//                 (std::forward<fn_t>(fn), std::forward<decltype(in[I])>(in[I]), std::forward<op_t>(op), std::forward<decltype(out[I])>(out[I])), ...);
//         }else{
//             (::bygo::op::apply<decltype(std::forward<fn_t>(fn)),
//                 decltype(std::forward<decltype(in[I])>(in[I])),
//                 decltype(std::forward<decltype(op[I])>(op[I])),
//                 decltype(std::forward<decltype(out[I])>(out[I])), typename shape_t::res_shape>
//                 (std::forward<fn_t>(fn), std::forward<decltype(in[I])>(in[I]), std::forward<decltype(op[I])>(op[I]), std::forward<decltype(out[I])>(out[I])), ...);
//         }
//     }
// }

// template <typename fn_t, typename in_t, typename op_t, typename out_t, typename shape_t = typename util::remove_cvref_t<out_t>::shape_type, typename Is = std::make_index_sequence<shape_t::dim>>
// constexpr auto apply(fn_t&& fn, in_t&& in, op_t&& op, out_t&& out){
//     if constexpr(aux::is_scalar_v<util::remove_cvref_t<out_t>>){
//         out = fn(in, op);
//     }else{

//         ::bygo::op::impl::apply<decltype(std::forward<fn_t>(fn)),
//             decltype(std::forward<in_t>(in)),
//             decltype(std::forward<op_t>(op)),
//             decltype(std::forward<out_t>(out)), shape_t>
//             (std::forward<fn_t>(fn), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out), Is{});
//     }
// }

namespace impl{
    struct _assign{
        template <typename in_t, typename op_t>
        constexpr auto operator()(in_t&& in, op_t&& op) -> std::common_type_t<in_t, op_t>{
            return op;
        }
    };

    template <typename in_t, typename op_t, typename out_t, typename axes_t, std::size_t ...I>
    constexpr auto assign(in_t&& in, op_t&& op, out_t&& out, axes_t axes, std::index_sequence<I...>){
        auto& out_part(out(std::get<I>(axes)...));
        ::bygo::op::apply<decltype(_assign()),
            decltype(in(std::get<I>(axes)...)),
            decltype(op(std::get<I>(axes)...)),
            decltype(out(std::get<I>(axes)...)),
            aux::nth_shape_t<typename util::remove_cvref_t<out_t>::shape_type, sizeof...(I)+1>>
        (_assign(), in(std::get<I>(axes)...), op(std::get<I>(axes)...), out_part);
    }

    template <typename in_t, typename op_t, typename out_t>
    constexpr auto assign(in_t&& in, op_t&& op, out_t&& out){
        // auto& out_part(out(std::get<I>(axes)...));
        ::bygo::op::apply(_assign(), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out));
        // ::bygo::op::apply<decltype(_assign()),
        //     decltype(in(std::get<I>(axes)...)),
        //     decltype(op(std::get<I>(axes)...)),
        //     decltype(out(std::get<I>(axes)...)),
        //     aux::nth_shape_t<typename util::remove_cvref_t<out_t>::shape_type, sizeof...(I)+1>>
        // (_assign(), in(std::get<I>(axes)...), op(std::get<I>(axes)...), out_part);
    }
}

template <typename in_t, typename op_t, typename ...axes_t, typename Is = std::make_index_sequence<sizeof...(axes_t)>>
constexpr auto assign(in_t&& in, op_t&& op, std::tuple<axes_t...> axes){
    using out_type = util::remove_cvref_t<in_t>;

    out_type res(in);
    impl::assign(std::forward<in_t>(in), std::forward<op_t>(op), res, axes, Is{});

    return res;
}

template <typename in_t, typename op_t>
constexpr auto assign(in_t&& in, op_t&& op){
    using out_type = util::remove_cvref_t<in_t>;

    out_type res(in);
    impl::assign(std::forward<in_t>(in), std::forward<op_t>(op), res);

    return res;
}

}

#endif