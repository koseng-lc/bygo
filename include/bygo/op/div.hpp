#ifndef BYGO_OP_DIV_HPP
#define BYGO_OP_DIV_HPP

#include <bygo/op/apply.hpp>
#include <bygo/prop/shape.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

namespace impl{
    struct _div{
        template <typename in_t, typename op_t>
        constexpr auto operator()(in_t&& in, op_t&& op) -> std::common_type_t<in_t, op_t>{
            return in / op;
        }
    };

    template <typename in_t, typename op_t, typename out_t, typename axes_t, std::size_t ...I>
    constexpr auto div(in_t&& in, op_t&& op, out_t&& out, axes_t axes, std::index_sequence<I...>){
        auto& out_part(out(std::get<I>(axes)...));
        using shape_t = aux::nth_shape_t<typename util::remove_cvref_t<out_t>::shape_type, sizeof...(I)+1>;
        if constexpr(aux::is_scalar_v<util::remove_cvref_t<op_t>>){
            ::bygo::op::apply<decltype(_div()),
                decltype(in(std::get<I>(axes)...)),
                decltype(op),
                decltype(out(std::get<I>(axes)...)),
                shape_t>
            (_div(), in(std::get<I>(axes)...), op, out_part);
        }else{
            ::bygo::op::apply<decltype(_div()),
                decltype(in(std::get<I>(axes)...)),
                decltype(op(std::get<I>(axes)...)),
                decltype(out(std::get<I>(axes)...)),
                shape_t>
            (_div(), in(std::get<I>(axes)...), op(std::get<I>(axes)...), out_part);
        }
    }
}

template <typename in_t, typename op_t, typename ...axes_t, typename Is = std::make_index_sequence<sizeof...(axes_t)>>
constexpr auto div(in_t&& in, op_t&& op, std::tuple<axes_t...> axes){
    using out_type = util::remove_cvref_t<in_t>;
    out_type res;
    impl::div(std::forward<in_t>(in), std::forward<op_t>(op), res, axes, Is{});

    return res;
}

template <typename in_t, typename op_t, typename out_t>
constexpr auto div(in_t&& in, op_t&& op, out_t&& out){
    apply(impl::_div(), in, op, out);
}

} // namespace bygo::op

#endif