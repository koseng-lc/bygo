#ifndef BYGO_OP_STACK_HPP
#define BYGO_OP_STACK_HPP

#include <bygo/basic/basic_elem.hpp>
#include <bygo/prop/shape.hpp>

namespace bygo::op{

namespace impl{

    template <typename tup_t, std::size_t ...I>
    constexpr auto _cut_last(tup_t&& tup, std::index_sequence<I...>){
        return std::make_tuple(std::get<I>(tup)...);
    }

    template <typename ...Ts>
    constexpr auto cut_last(Ts... ts){

        using Is = std::make_index_sequence<sizeof...(Ts)-1>;
        
        return _cut_last(std::forward_as_tuple(ts...), Is{});
    }

    template <auto idx, typename in_t, typename op_t>
    constexpr decltype(auto) select_op(in_t&& in, op_t&& op){

        if constexpr(idx){
            return std::forward<op_t>(op);
        }else{
            return std::forward<in_t>(in);
        }
    }

    template <typename shape_t, std::size_t axis, std::size_t idx
        , typename in_t, typename op_t, typename out_t
        , std::size_t ...I, typename ...axes_t>
    constexpr auto _stack(in_t&& in, op_t&& op, out_t&& out
        , std::index_sequence<I...>, axes_t ...axes){

        if constexpr(sizeof...(axes_t)-1 == axis){
            if constexpr(sizeof...(axes_t) == 1){
                if constexpr(axis == 0){
                    out = ::bygo::op::assign(std::forward<out_t>(out), select_op<idx>(std::forward<in_t>(in), std::forward<op_t>(op)), bygo::ax(axes...));
                }else{
                    out = ::bygo::op::assign(std::forward<out_t>(out), select_op<idx>(std::forward<in_t>(in), std::forward<op_t>(op)), bygo::ax(axes...), bygo::ax(axes...));
                }
            }else{
                out = ::bygo::op::assign(std::forward<out_t>(out), select_op<idx>(std::forward<in_t>(in), std::forward<op_t>(op)), bygo::ax(axes...), cut_last(axes...));
            }
        }else{

            using Is = std::make_index_sequence<shape_t::dim>;
            
            (_stack<typename shape_t::res_shape, axis, I>(std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out)
                , Is{}, axes..., I), ...);
        }
    }

    template <typename shape_t, std::size_t axis, typename in_t, typename op_t, typename out_t, std::size_t ...I>
    constexpr auto stack(in_t&& in, op_t&& op, out_t&& out, std::index_sequence<I...>){

        using Is = std::make_index_sequence<shape_t::dim>;

        (_stack<typename shape_t::res_shape, axis, I>(std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out), Is{}, I), ...);
    }
} // namespace impl

template <typename in_t, typename op_t, typename axis_t>
constexpr auto stack(in_t&& in, op_t&& op, axis_t&& axis){

    using in_type = util::remove_cvref_t<in_t>;
    using out_shape = aux::insert_axis_t<axis(), 2, typename in_type::shape_type>;
    using out_type = basic_elem<out_shape, typename in_type::scalar_type>;

    using Is = std::make_index_sequence<out_shape::dim>;

    out_type res{};
    impl::stack<typename out_shape::res_shape, axis()>(std::forward<in_t>(in), std::forward<op_t>(op), res, Is{});

    return res;
}

} // namespace bygo::op

#endif