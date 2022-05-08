#ifndef BYGO_OP_TRANSPOSE_HPP
#define BYGO_OP_TRANSPOSE_HPP

#include <bygo/basic.hpp>
#include <bygo/prop/shape.hpp>

namespace bygo::op{

namespace impl{
    
    /**
     *  @brief Set the i,j element of output with j,i element of the input
     *  @note Support matrix only
     */ 
    template <typename in_t, typename out_t, std::size_t ...I>
    constexpr auto _transpose(in_t&& in, out_t&& out, std::index_sequence<I...>){

        using out_type = typename util::remove_cvref_t<out_t>;

        constexpr auto nrows{out_type::nrows};

        ((out[I%nrows][I/nrows] = in[I/nrows][I%nrows]), ...);
    }

    template <typename in_t, typename out_t, typename Is = std::make_index_sequence<util::remove_cvref_t<out_t>::nelem>>
    constexpr auto transpose(in_t&& in, out_t&& out){
        _transpose(std::forward<in_t>(in), std::forward<out_t>(out), Is{});
    }
} // namespace impl

/**
 *  @brief Transpose of matrix
 *  @note Support matrix only
 */ 
template <typename in_t>
constexpr auto transpose(in_t&& in){

    using out_type = typename util::remove_cvref_t<in_t>::transpose_type;

    out_type res;
    impl::transpose(std::forward<in_t>(in), res);

    return res;
}

} //namespace bygo::op

#endif