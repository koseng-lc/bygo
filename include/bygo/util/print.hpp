#ifndef BYGO_UTIL_PRINT_HPP
#define BYGO_UTIL_PRINT_HPP

#include <bygo/prop/shape.hpp>
#include <bygo/util/util.hpp>

namespace bygo::util{

template <typename in_t>
auto print_matrix(in_t&& in){
    using in_type = util::remove_cvref_t<in_t>;
    for(auto i(0); i < in_type::nrows; i++){
        for(auto j(0); j < in_type::ncols; j++){
            std::cout << in(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

namespace impl{
    template <typename shape_t, typename in_t, std::size_t ...I>
    constexpr void print(in_t&& in, std::index_sequence<I...>, std::string&& separator = ""){
        using in_type = util::remove_cvref_t<in_t>;
        using res_shape = typename shape_t::res_shape;
        using Is = std::make_index_sequence<res_shape::dim>;

        if constexpr(aux::is_scalar_v<in_type>){
            std::cout << in << separator;
        }else{
            if constexpr((aux::is_scalar_v<util::remove_cvref_t<decltype(in[I])>> && ...)){
                std::cout << std::endl << "  ";
            }
            std::cout << "[";
            (print<res_shape>(in[I], Is{}, (I == sizeof...(I)-1) ? "" : " "), ...);
            std::cout << "]";
        }
    }
}

template <typename in_t>
constexpr auto print(in_t&& in){
    using in_type = util::remove_cvref_t<in_t>;
    using shape_type = typename in_type::shape_type;
    using Is = std::make_index_sequence<shape_type::dim>;
    
    impl::print<shape_type>(std::forward<in_t>(in), Is{});
    std::cout << std::endl;
}

} // namespace bygo::util

#endif