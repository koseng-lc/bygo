#ifndef BYGO_OP_CONCAT_HPP
#define BYGO_OP_CONCAT_HPP

#include <bygo/basic/basic_elem.hpp>
#include <bygo/prop/shape.hpp>

namespace bygo::op{

namespace impl{

    template <std::size_t S, typename tup_t, std::size_t ...I>
    constexpr auto _shift_last(tup_t&& tup, std::index_sequence<I...>){
        return std::make_tuple((std::get<I>(tup) - (std::size_t)(I == sizeof...(I)-1)*S)...);
    }

    template <std::size_t S, typename ...Ts>
    constexpr auto shift_last(Ts... ts){

        using Is = std::make_index_sequence<sizeof...(Ts)>;

        return _shift_last<S>(std::forward_as_tuple(ts...), Is{});
    }

    template <typename shape_t, std::size_t axis, std::size_t idx, typename in_t, typename op_t, typename out_t, std::size_t ...J, typename ...axes_t>
    constexpr auto _concat(in_t&& in, op_t&& op, out_t&& out, std::index_sequence<J...>, axes_t ...axes){

        using Js = std::make_index_sequence<shape_t::dim>;
        using out_shape = typename util::remove_cvref_t<in_t>::shape_type;

        constexpr auto curr_dim{aux::nth_shape_dim_v<out_shape, sizeof...(axes_t)>};

        if constexpr(sizeof...(axes_t)-1 == axis){
            if constexpr(idx < curr_dim){
                out = ::bygo::op::assign(std::forward<out_t>(out), std::forward<in_t>(in), bygo::ax(axes...), bygo::ax(axes...));
            }else{
                out = ::bygo::op::assign(std::forward<out_t>(out), std::forward<op_t>(op), bygo::ax(axes...), shift_last<curr_dim>(axes...));
            }
        }else{
            (_concat<typename shape_t::res_shape, axis, J>(std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out), Js{}, axes..., J), ...);
        }
    }

    template <typename shape_t, std::size_t axis, typename in_t, typename op_t, typename out_t, std::size_t ...I>
    constexpr auto concat(in_t&& in, op_t&& op, out_t&& out, std::index_sequence<I...>){

        using Is = std::make_index_sequence<shape_t::dim>;

        (_concat<typename shape_t::res_shape, axis, I>(std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out), Is{}, I), ...);
    }
} // namespace impl

template <typename in_t, typename op_t, typename axis_t>
constexpr auto concat(in_t&& in, op_t&& op, axis_t&& axis){

    using in_type = util::remove_cvref_t<in_t>;
    using op_type = util::remove_cvref_t<op_t>;
    using out_shape = aux::add_nth_shape_t<axis(), aux::nth_shape_dim_v<typename op_type::shape_type, axis()+1>, typename in_type::shape_type>;
    using out_type = basic_elem<out_shape, typename in_type::scalar_type>;

    using Is = std::make_index_sequence<out_shape::dim>;

    out_type res{};
    impl::concat<typename out_shape::res_shape, axis()>(std::forward<in_t>(in), std::forward<op_t>(op), res, Is{});

    return res;
}

} // namespace bygo::op

#endif