#ifndef BYGO_SHAPE_HPP
#define BYGO_SHAPE_HPP

#include <tuple>

#include <bygo/aux/aux.hpp>

namespace bygo{

using uint_t = unsigned int;

template <std::size_t d, std::size_t ...ds>
struct shape{
    static constexpr std::size_t size{sizeof...(ds) + 1};
    using res_shape = shape<ds...>;
    static constexpr std::size_t dim{d};
    static constexpr std::size_t nelem{aux::nelem_v<d, ds...>};
};

template <std::size_t d>
struct shape<d>{
    static constexpr std::size_t size{1};
    using res_shape = shape<0>;
    static constexpr std::size_t dim{d};
    static constexpr std::size_t nelem{aux::nelem_v<d>};
};

namespace aux{

/**
 *  @brief Check whether the shape is equal or not
 */ 
template <typename S1, typename S2>
struct is_shape_equal{
    static constexpr auto value = (S1::dim == S2::dim) & is_shape_equal<typename S1::res_shape, typename S2::res_shape>::value;
};

template <typename S>
struct is_shape_equal<shape<0>, S>{
    static constexpr auto value = false;
};

template <typename S>
struct is_shape_equal<S, shape<0>>{
    static constexpr auto value = false;
};

template <>
struct is_shape_equal<shape<0>, shape<0>>{
    static constexpr auto value = true;
};

template <typename S1, typename S2>
static constexpr auto is_shape_equal_v = is_shape_equal<S1, S2>::value;

/**
 *  @brief Get n-th shape
 */ 
template <typename shape_t, std::size_t N>
struct nth_shape{
    using type = typename nth_shape<typename shape_t::res_shape, N-1>::type;
};

template <typename shape_t>
struct nth_shape<shape_t, 1>{
    using type = shape_t;
};

template <typename shape_t, std::size_t N>
using nth_shape_t = typename nth_shape<shape_t, N>::type;

/**
 *  @brief Get n-th shape dim
 */ 
template <typename shape_t, std::size_t N>
struct nth_shape_dim{
    static constexpr auto value = nth_shape_t<shape_t, N>::dim;
};

template <typename shape_t, std::size_t N>
static constexpr auto nth_shape_dim_v = nth_shape_dim<shape_t, N>::value;

/**
 *  @brief Get overall shape dim
 */

template <typename shape_t, std::size_t ...Ds>
struct shape_dim{
    using type = typename shape_dim<typename shape_t::res_shape, Ds..., shape_t::dim>::type;
};

template <std::size_t ...Ds>
struct shape_dim<shape<0>, Ds...>{
    using type = std::index_sequence<Ds...>;
};

template <typename shape_t>
using shape_dim_t = typename shape_dim<shape_t>::type;

/**
 *  @brief Set n-th shape
 */

namespace impl{
    template <std::size_t N, std::size_t V, std::size_t ...I, std::size_t ...J>
    constexpr auto set_nth_shape(std::index_sequence<I...>, std::index_sequence<J...>){
        return shape<(I * (std::size_t)(J != N) + (std::size_t)(J == N) * V)...>{};
    }

    template <std::size_t N, std::size_t V, std::size_t ...I, std::size_t ...J>
    constexpr auto add_nth_shape(std::index_sequence<I...>, std::index_sequence<J...>){
        return shape<(I + (std::size_t)(J == N) * V)...>{};
    }
}

template <std::size_t N, std::size_t V, typename shape_t>
struct set_nth_shape{
private:
    using Is = shape_dim_t<shape_t>;
    using Js = std::make_index_sequence<shape_t::size>;
public:
    using type = decltype(impl::set_nth_shape<N, V>(Is{}, Js{}));
};

template <std::size_t N, std::size_t V, typename shape_t>
struct add_nth_shape{
private:
    using Is = shape_dim_t<shape_t>;
    using Js = std::make_index_sequence<shape_t::size>;
public:
    using type = decltype(impl::add_nth_shape<N, V>(Is{}, Js{}));
};

template <std::size_t N, std::size_t V, typename shape_t>
using set_nth_shape_t = typename set_nth_shape<N, V, shape_t>::type;

template <std::size_t N, std::size_t V, typename shape_t>
using add_nth_shape_t = typename add_nth_shape<N, V, shape_t>::type;

}

}

#endif