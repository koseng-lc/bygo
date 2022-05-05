#ifndef BYGO_OP_APPLY_HPP
#define BYGO_OP_APPLY_HPP

#include <optional>
#include <iostream>

#include <bygo/util/util.hpp>
#include <bygo/aux/aux.hpp>

namespace bygo::op{

namespace impl{
    /**
     * @note If we use the universal reference on op_t and in_t, the deduced type include const which cause implicit conversion (?)
     * @todo reduce the boiler plate code
     */
    template <typename shape_t, typename fn_t, typename in_t, typename op_t, typename out_t
        , std::size_t... I, typename axes1_t, typename axes2_t, std::size_t... J, std::size_t... K, typename ...axes3_t>
    constexpr void _apply(fn_t&& fn, in_t in, op_t op, out_t&& out
        , std::index_sequence<I...>, axes1_t axes1, axes2_t axes2, std::index_sequence<J...>, std::index_sequence<K...>, axes3_t... axes3){

        using axes1_type = util::remove_cvref_t<axes1_t>;
        using axes2_type = util::remove_cvref_t<axes2_t>;

        if constexpr(std::is_same<axes1_type, whole_axes_t>::value & std::is_same<axes2_type, whole_axes_t>::value){
            
            if constexpr(aux::is_scalar_v<util::remove_cvref_t<decltype(out(axes3...))>>){
                if constexpr(aux::is_scalar_v<util::remove_cvref_t<op_t>>){
                    out(axes3...) = fn(in(axes3...), op);
                }else{
                    out(axes3...) = fn(in(axes3...), op(axes3...));
                }
            }else{
                
                using Is = std::make_index_sequence<shape_t::dim>;

                (_apply<typename shape_t::res_shape>
                    (std::forward<fn_t>(fn), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out)
                    , Is{}, axes1, axes2, std::index_sequence<J...>{}, std::index_sequence<K...>{}, axes3..., I), ...);
            }
        }else if constexpr(std::is_same<axes2_type, whole_axes_t>::value){

            if constexpr(aux::is_scalar_v<util::remove_cvref_t<decltype(out(std::get<J>(axes1)..., axes3...))>>){
                if constexpr(aux::is_scalar_v<util::remove_cvref_t<op_t>>){
                    out(std::get<J>(axes1)..., axes3...) = fn(in(std::get<J>(axes1)..., axes3...), op);
                }else{
                    out(std::get<J>(axes1)..., axes3...) = fn(in(std::get<J>(axes1)..., axes3...), op(axes3...));
                }
            }else{
                
                using Is = std::make_index_sequence<shape_t::dim>;

                (_apply<typename shape_t::res_shape>
                    (std::forward<fn_t>(fn), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out)
                    , Is{}, axes1, axes2, std::index_sequence<J...>{}, std::index_sequence<K...>{}, axes3..., I), ...);
            }
        }else if constexpr(std::is_same<axes1_type, whole_axes_t>::value){

            if constexpr(aux::is_scalar_v<util::remove_cvref_t<decltype(out(axes3...))>>){
                if constexpr(aux::is_scalar_v<util::remove_cvref_t<op_t>>){
                    out(axes3...) = fn(in(axes3...), op);
                }else{
                    out(axes3...) = fn(in(axes3...), op(std::get<K>(axes2)..., axes3...));
                }
            }else{
                
                using Is = std::make_index_sequence<shape_t::dim>;

                (_apply<typename shape_t::res_shape>
                    (std::forward<fn_t>(fn), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out)
                    , Is{}, axes1, axes2, std::index_sequence<J...>{}, std::index_sequence<K...>{}, axes3..., I), ...);
            }
        }else{

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
    }

    template <typename shape_t, typename fn_t, typename in_t, typename op_t, typename out_t, typename axes1_t, typename axes2_t, std::size_t... I, std::size_t... J>
    constexpr void apply(fn_t&& fn, in_t&& in, op_t&& op, out_t&& out, axes1_t axes1, axes2_t axes2, std::index_sequence<I...>, std::index_sequence<J...>){

        using Is = std::make_index_sequence<shape_t::dim>;

        _apply<typename shape_t::res_shape>
            (std::forward<fn_t>(fn), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out)
            , Is{}, std::forward<axes1_t>(axes1), std::forward<axes2_t>(axes2), std::index_sequence<I...>{}, std::index_sequence<J...>{});
    }
}

template <typename fn_t, typename in_t, typename op_t, typename out_t, typename axes1_t, typename axes2_t>
constexpr auto apply(fn_t&& fn, in_t&& in, op_t&& op, out_t&& out
                , axes1_t&& axes1 = whole_axes_t{}, axes2_t&& axes2 = whole_axes_t{}){

    using out_shape = typename util::remove_cvref_t<out_t>::shape_type;
    using op_shape = typename util::remove_cvref_t<op_t>::shape_type;

    // constexpr auto swap_axes{is_whole_axes_v<axes2_t> & (out_shape::dim > op_shape::dim)};

    // using axes1_type = std::conditional_t<!swap_axes, util::remove_cvref_t<axes2_t>, util::remove_cvref_t<axes1_t>>;
    // using axes2_type = std::conditional_t<swap_axes, util::remove_cvref_t<axes2_t>, util::remove_cvref_t<axes1_t>>;
    using axes1_type = util::remove_cvref_t<axes1_t>;
    using axes2_type = util::remove_cvref_t<axes2_t>;
    constexpr auto axes1_size = std::tuple_size_v<axes1_type>;
    constexpr auto axes2_size = std::tuple_size_v<axes2_type>;

    
    using target_out_shape = aux::nth_shape_t<out_shape, axes1_size + 1>;
    using Is = std::make_index_sequence<axes1_size>;

    using target_op_shape = aux::nth_shape_t<op_shape, axes2_size + 1>;
    using Js = std::make_index_sequence<axes2_size>;

    impl::apply<target_out_shape>
        (std::forward<fn_t>(fn), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out)
        , std::forward<axes1_t>(axes1), std::forward<axes2_t>(axes2), Is{}, Js{});
}

} // namespace bygo::op

#endif