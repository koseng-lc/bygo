#ifndef BYGO_OP_DOT_HPP
#define BYGO_OP_DOT_HPP

#include <bygo/basic.hpp>
#include <bygo/prop/shape.hpp>

namespace bygo::op{

namespace impl{
    /**
     *  @brief Dot product between r-th row of input and c-th column of operand
     *  @note Support matrix only
     */ 
    template <typename in_t, typename op_t, typename row_t, typename col_t, std::size_t ...I>
    constexpr auto dot_vec(in_t&& in, op_t&& op, row_t r, col_t c, std::index_sequence<I...>){
        return ((in[r][I] * op[I][c]) + ...);
    }

    /**
     *  @brief Set the element of the output
     *  @note Support matrix only
     */ 
    template <typename in_t, typename op_t, typename out_t, std::size_t ...I, typename Is = std::make_index_sequence<util::remove_cvref_t<in_t>::ncols>>
    constexpr auto _dot(in_t&& in, op_t&& op, out_t&& out, std::index_sequence<I...>){
        using out_type = typename util::remove_cvref_t<out_t>;
        
        constexpr auto nrows{out_type::nrows};

        ((out[I%nrows][I/nrows] = dot_vec(std::forward<in_t>(in), std::forward<op_t>(op), I%nrows, I/nrows, Is{})), ...);
    }

    template <typename in_t, typename op_t, typename out_t, typename Is = std::make_index_sequence<util::remove_cvref_t<out_t>::nelem>>
    constexpr auto dot(in_t&& in, op_t&& op, out_t&& out){
        _dot(std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out), Is{});
    }
}

/**
 *  @brief Dot product of matrix
 *  @note Support matrix only
 */ 
template <typename in_t, typename op_t>
constexpr auto dot(in_t&& in, op_t&& op){
    using in_type = util::remove_cvref_t<in_t>;
    using op_type = util::remove_cvref_t<op_t>;
    using out_t = ::bygo::matrix<in_type::nrows, op_type::ncols, typename in_type::scalar_type>;
    out_t res;

    impl::dot(std::forward<in_t>(in), std::forward<op_t>(op), res);

    return res;
}

} // namespace bygo::op

#endif