#ifndef BYGO_UTIL_UTIL_HPP
#define BYGO_UTIL_UTIL_HPP

namespace bygo::util{

    /**
     *  @brief Remove constant and/or reference
     */ 
    template <typename T>
    struct remove_cvref{
        using type = std::remove_cv_t<std::remove_reference_t<T>>;
    };

    template <typename T>
    using remove_cvref_t = typename remove_cvref<T>::type;

    /**
     *  @brief For-loop in compile-time
     */
    namespace impl{

        template <typename fn_t, typename tup_t, std::size_t ...I, std::size_t ...J>
        constexpr auto for_loop(fn_t&& fn, tup_t&& tup, std::index_sequence<I...>, std::index_sequence<J...>){
            (fn(std::forward<std::tuple_element_t<J, tup_t>>(std::get<J>(tup))..., I), ...);
        }
    }

    template <std::size_t n_iter, typename fn_t, typename ...args_t,
        typename Is = std::make_index_sequence<n_iter>,
        typename Js = std::make_index_sequence<sizeof...(args_t)>>
    constexpr auto for_loop(fn_t&& fn, args_t&&... args){
        impl::for_loop(std::forward<fn_t>(fn), std::forward_as_tuple(args...), Is{}, Js{});
    }

    /**
     *  @brief Find max value between two arguments
     */
    template <typename val1_t, typename val2_t>
    constexpr auto max(val1_t val1, val2_t val2){
        if constexpr(val1 > val2){
            return val1;
        }else{
            return val2;
        }
    }
    
    /**
     *  @brief Find max value between two arguments
     */
    template <typename val1_t, typename val2_t>
    constexpr auto min(val1_t val1, val2_t val2){
        if constexpr(val1 < val2){
            return val1;
        }else{
            return val2;
        }
    }
    
} // namespace bygo::util

#endif