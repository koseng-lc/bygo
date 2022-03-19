#ifndef BYGO_OP_ASSIGN_HPP
#define BYGO_OP_ASSIGN_HPP

#include <bygo/op/apply.hpp>
#include <bygo/aux/aux.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

namespace impl{
    struct _assign{
        template <typename in_t, typename op_t>
        constexpr auto operator()(in_t&& in, op_t&& op) -> std::common_type_t<in_t, op_t>{
            return op;
        }
    };

    template <typename in_t, typename op_t, typename out_t, typename axes1_t, typename axes2_t, std::size_t ...I, std::size_t ...J>
    constexpr auto assign(in_t&& in, op_t&& op, out_t&& out, axes1_t axes1, axes2_t axes2, std::index_sequence<I...>, std::index_sequence<J...>){
        auto& out_part(out(std::get<I>(axes1)...));

        ::bygo::op::apply<decltype(_assign()),
            decltype(in(std::get<I>(axes1)...)),
            decltype(op(std::get<J>(axes2)...)),
            decltype(out_part),
            aux::nth_shape_t<typename util::remove_cvref_t<out_t>::shape_type, sizeof...(I)+1>>
        (_assign(), in(std::get<I>(axes1)...), op(std::get<J>(axes2)...), out_part);
    }

    template <typename in_t, typename op_t, typename out_t, typename axes_t, std::size_t ...I>
    constexpr auto assign(in_t&& in, op_t&& op, out_t&& out, axes_t axes, std::index_sequence<I...>){
        auto& out_part(out(std::get<I>(axes)...));

        ::bygo::op::apply<decltype(_assign()),
            decltype(in(std::get<I>(axes)...)),
            decltype(std::forward<op_t>(op)),
            decltype(out_part),
            aux::nth_shape_t<typename util::remove_cvref_t<out_t>::shape_type, sizeof...(I)+1>>
        (_assign(), in(std::get<I>(axes)...), std::forward<op_t>(op), out_part);
    }

    template <typename in_t, typename op_t, typename out_t>
    constexpr auto assign(in_t&& in, op_t&& op, out_t&& out){
        ::bygo::op::apply(_assign(), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out));
    }
}

template <typename in_t, typename op_t, typename ...axes1_t, typename ...axes2_t>
constexpr auto assign(in_t&& in, op_t&& op, std::tuple<axes1_t...> axes1, std::tuple<axes2_t...> axes2){
    using out_type = util::remove_cvref_t<in_t>;
    using Is = std::make_index_sequence<sizeof...(axes1_t)>;
    using Js = std::make_index_sequence<sizeof...(axes2_t)>;
    out_type res(in);
    impl::assign(std::forward<in_t>(in), std::forward<op_t>(op), res, axes1, axes2, Is{}, Js{});

    return res;
}

template <typename in_t, typename op_t, typename ...axes_t>
constexpr auto assign(in_t&& in, op_t&& op, std::tuple<axes_t...> axes){
    using in_type = util::remove_cvref_t<in_t>;
    using op_type = util::remove_cvref_t<op_t>;
    
    // static_assert(aux::is_shape_equal_v<sub_shape, typename op_type::shape_type>, "Shape is equal!");
    if constexpr(aux::is_shape_equal_v<typename in_type::shape_type, typename op_type::shape_type>){
        return assign(std::forward<in_t>(in), std::forward<op_t>(op), axes, axes);
    }else{
        using sub_shape = aux::nth_shape_t<typename in_type::shape_type, sizeof...(axes_t)>;
        using out_type = util::remove_cvref_t<in_t>;
        using Is = std::make_index_sequence<sizeof...(axes_t)>;

        out_type res(in);
        impl::assign(std::forward<in_t>(in), std::forward<op_t>(op), res, axes, Is{});

        return res;
    }
    
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