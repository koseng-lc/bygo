#ifndef BYGO_UTIL_PRINT_HPP
#define BYGO_UTIL_PRINT_HPP

#include <bygo/prop/shape.hpp>
#include <bygo/util/util.hpp>

#include <iostream>

namespace bygo::util{

namespace impl{
    template <std::size_t idx, std::size_t prev_dim, std::size_t depth, typename shape_t, typename in_t, std::size_t ...I>
    void _print(in_t&& in, std::index_sequence<I...>, std::string&& separator = ""){

        using in_type = util::remove_cvref_t<in_t>;

        if constexpr(aux::is_scalar_v<in_type>){
            std::cout << in << separator;
        }else{

            using Is = std::make_index_sequence<shape_t::dim>;

            std::cout << "[";
            (_print<I, sizeof...(I), depth + 1, typename shape_t::res_shape>(in[I], Is{}, (I == sizeof...(I)-1) ? "" : " "), ...);
            std::cout << "]";

            if constexpr(idx < prev_dim-1){
                std::cout << std::endl;
                for(std::size_t i(0); i < depth; i++){
                    std::cout << " ";
                }
            }
        }
    }

    template <typename shape_t, typename in_t, std::size_t ...I>
    void print(in_t&& in, std::index_sequence<I...>){

        using Is = std::make_index_sequence<shape_t::dim>;

        std::cout << "[";
        (_print<I, sizeof...(I), 1, typename shape_t::res_shape>(in[I], Is{}), ...);
        std::cout << "]";
    }
}

template <typename in_t>
auto print(in_t&& in){

    using in_type = util::remove_cvref_t<in_t>;
    using shape_type = typename in_type::shape_type;
    using Is = std::make_index_sequence<shape_type::dim>;
    
    impl::print<typename shape_type::res_shape>(std::forward<in_t>(in), Is{});
    std::cout << std::endl;
}

} // namespace bygo::util

#endif