#ifndef BYGO_OP_ADD_HPP
#define BYGO_OP_ADD_HPP

#include <bygo/op/apply.hpp>
#include <bygo/prop/shape.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

namespace impl{
    struct _add{
        template <typename in_t, typename op_t>
        constexpr auto operator()(in_t&& in, op_t&& op) -> std::common_type_t<in_t, op_t>{
            return in + op;
        }
    };

    template <typename in_t, typename op_t, typename out_t, typename axes1_t, typename axes2_t, std::size_t ...I>
    constexpr auto add(in_t&& in, op_t&& op, out_t&& out, axes1_t axes1, axes2_t axes2, std::index_sequence<I...>){
        auto& out_part(out(std::get<I>(axes1)...));
        // auto op_part()
        using shape_t = aux::nth_shape_t<typename util::remove_cvref_t<out_t>::shape_type, sizeof...(I)+1>;
        if constexpr(aux::is_scalar_v<util::remove_cvref_t<op_t>>){
            ::bygo::op::apply<decltype(_add()),
                decltype(in(std::get<I>(axes1)...)),
                decltype(std::forward<op_t>(op)),
                decltype(out(std::get<I>(axes1)...)),
                shape_t>
            (_add(), in(std::get<I>(axes1)...), std::forward<op_t>(op), out_part);
        }else{
            ::bygo::op::apply<decltype(_add()),
                decltype(in(std::get<I>(axes1)...)),
                decltype(op(std::get<I>(axes2)...)),
                decltype(out(std::get<I>(axes1)...)),
                shape_t>
            (_add(), in(std::get<I>(axes1)...), op(std::get<I>(axes2)...), out_part);
        }
    }
}

template <typename in_t, typename op_t, typename ...axes1_t, typename ...axes2_t, typename Is = std::make_index_sequence<sizeof...(axes1_t)>>
constexpr auto add(in_t&& in, op_t&& op, std::tuple<axes1_t...> axes1, std::tuple<axes2_t...> axes2){
    using out_type = util::remove_cvref_t<in_t>;
    out_type res(in);
    impl::add(std::forward<in_t>(in), std::forward<op_t>(op), res, axes1, axes2, Is{});

    return res;
}

template <typename in_t, typename op_t, typename ...axes_t>
constexpr auto add(in_t&& in, op_t&& op, std::tuple<axes_t...> axes){
    return add(std::forward<in_t>(in), std::forward<op_t>(op), axes, axes);
}

template <typename in_t, typename op_t>
constexpr auto add(in_t&& in, op_t&& op){
    using out_type = util::remove_cvref_t<in_t>;

    out_type res(in);

    apply(impl::_add(), std::forward<in_t>(in), std::forward<op_t>(op), res);

    return res;
}

} // namespace bygo::op

#endif