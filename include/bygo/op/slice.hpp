#ifndef BYGO_OP_SLICE_HPP
#define BYGO_OP_SLICE_HPP

#include <bygo/basic/basic_elem.hpp>

namespace bygo::op{

namespace impl{

    template <typename arg_t>
    constexpr auto lb(arg_t&& arg){
        
        return std::get<0>(arg);
    }

    template <typename arg_t>
    constexpr auto ub(arg_t&& arg){

        return std::get<1>(arg);
    }

    template <std::size_t I, typename shape_t, typename ival_t>
    constexpr auto d(ival_t&& ival){

        if (ub(ival) < 0 || lb(ival) < 0){
            return (int)aux::nth_shape_dim_v<shape_t, I>;
        }else{
            return ub(ival) - lb(ival) + 1;
        }        
    }

    template <typename shape_t, std::size_t... I, std::size_t... J, typename ival_t, typename ...ivals_t>
    constexpr auto deduce_shape(std::index_sequence<I...>, std::index_sequence<J...>, ival_t&& ival, ivals_t&&... ivals){

        return ::bygo::shape<d<1, shape_t>(ival()), (d<I+2, shape_t>(ivals()))..., aux::nth_shape_dim_v<shape_t, sizeof...(ivals_t) + J + 2>...>{};
    }

    template <std::size_t... I, typename ival_t, typename ...ivals_t>
    constexpr auto gen_lower(std::index_sequence<I...>, ival_t&& ival, ivals_t&&... ivals){

        return std::make_tuple(((lb(ival()) < 0) ? 0 : lb(ival())), ((lb(ivals()) < 0) ? 0 : lb(ivals()))..., (I - I)...);
    }

    template <typename shape_t, typename in_t, typename out_t, typename lower_t, std::size_t... I, std::size_t... J, typename ...indices_t>
    constexpr auto _slice(in_t in, out_t&& out, lower_t&& lower, std::index_sequence<I...>, std::index_sequence<J...>, indices_t... indices){

        if constexpr(aux::is_scalar_v<util::remove_cvref_t<decltype(in(indices...))>>){
            out(indices...) = in((std::get<J>(lower) + indices)...);
        }else{
            using Is = std::make_index_sequence<shape_t::dim>;
            (_slice<typename shape_t::res_shape>(std::forward<in_t>(in), std::forward<out_t>(out), std::forward<lower_t>(lower)
                , Is{}, std::index_sequence<J...>{}, indices..., I), ...);
        }
    }

    template <typename shape_t, typename in_t, typename out_t, typename lower_t, std::size_t... I, typename ...indices_t>
    constexpr auto slice(in_t&& in, out_t&& out, lower_t&& lower, std::index_sequence<I...>){

        using Is = std::make_index_sequence<shape_t::dim>;
        using Js = std::make_index_sequence<util::remove_cvref_t<out_t>::shape_type::size>;
        
        (_slice<typename shape_t::res_shape>(std::forward<in_t>(in), std::forward<out_t>(out), std::forward<lower_t>(lower), Is{}, Js{},  I), ...);
    }
}

template <typename in_t, typename ival_t, typename ...ivals_t>
constexpr auto slice(in_t&& in, ival_t&& ival, ivals_t&&... ivals){

    using in_type = util::remove_cvref_t<in_t>;
    using in_shape = typename in_type::shape_type;
    using out_shape = decltype(impl::deduce_shape<in_shape>(std::make_index_sequence<sizeof...(ivals_t)>{}
        , std::make_index_sequence<in_shape::size - sizeof...(ivals_t) - 1>{}
        , std::forward<ival_t>(ival), std::forward<ivals_t>(ivals)...));
    using out_type = bygo::basic_elem<out_shape, typename in_type::scalar_type>;

    out_type res{};
    impl::slice<typename out_shape::res_shape>(std::forward<in_t>(in), res
        , impl::gen_lower(std::make_index_sequence<in_shape::size - sizeof...(ivals_t) - 1>{}, std::forward<ival_t>(ival), std::forward<ivals_t>(ivals)...), std::make_index_sequence<out_shape::dim>{});

    return res;
}

}

#endif