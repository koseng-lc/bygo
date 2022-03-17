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
using set_nth_shape_t = typename set_nth_shape<N, V, shape_t>::type;

/**
 *  @brief Add n-th shape
 */

namespace impl{
    template <std::size_t N, std::size_t V, std::size_t ...I, std::size_t ...J>
    constexpr auto add_nth_shape(std::index_sequence<I...>, std::index_sequence<J...>){
        return shape<(I + (std::size_t)(J == N) * V)...>{};
    }
}

template <std::size_t N, std::size_t V, typename shape_t>
struct add_nth_shape{
private:
    using Is = shape_dim_t<shape_t>;
    using Js = std::make_index_sequence<shape_t::size>;
public:
    using type = decltype(impl::add_nth_shape<N, V>(Is{}, Js{}));
};

template <std::size_t N, std::size_t V, typename shape_t>
using add_nth_shape_t = typename add_nth_shape<N, V, shape_t>::type;

/**
 *  @brief Insert axis
 */

namespace impl{
   template <auto N, std::size_t V, std::size_t I, typename shape_t, std::size_t ...D>
    struct insert_axis{
        static constexpr auto put_here{(I == N) & (V != -1)};
        using type = typename insert_axis<N, (put_here ? -1 : V), I+1, std::conditional_t<put_here, shape_t, typename shape_t::res_shape>, D..., (put_here ? V : shape_t::dim)>::type;
    };

    template <auto N, std::size_t V, std::size_t I, std::size_t ...D>
    struct insert_axis<N, V, I, shape<0>, D...>{
        using type = shape<D...>;
    };
}

template <auto N, std::size_t V, typename shape_t>
struct insert_axis{
    using type = typename impl::insert_axis<N, V, 0, shape_t>::type;
};



template <auto N, std::size_t V, typename shape_t>
using insert_axis_t = typename insert_axis<N, V, shape_t>::type;

/**
 *  @brief Count the number of elements to the given axes
 */
template <std::size_t N, typename shape_t>
struct nth_nelem{
    static constexpr auto value = nth_shape_dim_v<shape_t, N> * nth_nelem<N-1, shape_t>::value;
};

template <typename shape_t>
struct nth_nelem<1, shape_t>{
    static constexpr auto value = nth_shape_dim_v<shape_t, 1>;
};

template <typename shape_t>
struct nth_nelem<0, shape_t>{
    static constexpr auto value = 1;
};

template <std::size_t N, typename shape_t>
static constexpr auto nth_nelem_v = nth_nelem<N, shape_t>::value;

/**
 *  @brief Count the number of elements to the given axes in reverse order
 */
namespace impl{
    template <std::size_t N, typename shape_t, std::size_t I>
    struct nth_nelem_rev{
        static constexpr auto value = nth_shape_dim_v<shape_t, shape_t::size-N> * nth_nelem_rev<N+1, shape_t, I-1>::value;
    };

    template <std::size_t N, typename shape_t>
    struct nth_nelem_rev<N, shape_t, 1>{
        static constexpr auto value = nth_shape_dim_v<shape_t, shape_t::size-N>;
    };

    template <std::size_t N, typename shape_t>
    struct nth_nelem_rev<N, shape_t, 0>{
        static constexpr auto value = 1;
    };
}

template <std::size_t N, typename shape_t>
struct nth_nelem_rev{
    static constexpr auto value = impl::nth_nelem_rev<0, shape_t, N>::value;
};

template <std::size_t N, typename shape_t>
static constexpr auto nth_nelem_rev_v = nth_nelem_rev<N, shape_t>::value;

/**
 *  @brief Access multi-dimensional array in single index fashion
 */
namespace impl{
    template <typename shape_t, std::size_t S, std::size_t Ax, std::size_t ...Axs>
    struct to_single{
        static constexpr auto value = Ax * nth_nelem_rev_v<S, shape_t> + to_single<shape_t, S-1, Axs...>::value;
    };

    template <typename shape_t, std::size_t S, std::size_t Ax>
    struct to_single<shape_t, S, Ax>{
        static constexpr auto value = Ax * nth_nelem_rev_v<S, shape_t>;
    };
}

template <typename shape_t, std::size_t Ax, std::size_t ...Axs>
struct to_single{
    // static constexpr auto value = impl::to_single<shape_t, shape_t::size-1, Ax, Axs...>::value;
    static constexpr auto value = impl::to_single<shape_t, sizeof...(Axs), Ax, Axs...>::value;
};

template <typename shape_t, std::size_t Ax, std::size_t ...Axs>
static constexpr auto to_single_v = to_single<shape_t, Ax, Axs...>::value;

/**
 *  @brief Access multi-dimensional array in normal way
 */
namespace impl{
    template <typename shape_t, typename terminate_t, std::size_t I, std::size_t ...Ds>
    struct to_multi{
        using type = typename to_multi<shape_t, typename terminate_t::res_shape, I, ((I - to_single_v<shape_t, Ds...>)/(nth_nelem_rev_v<sizeof...(Ds), shape_t>)) % nth_shape_dim_v<shape_t, shape_t::size-sizeof...(Ds)>, Ds...>::type;
    };

    template <typename shape_t, std::size_t I, std::size_t ...Ds>
    struct to_multi<shape_t, shape<0>, I, Ds...>{
        using type = std::index_sequence<Ds...>;
    };
}

template <typename shape_t, std::size_t I>
struct to_multi{
    using type = typename impl::to_multi<shape_t, typename shape_t::res_shape, I, I % nth_shape_dim_v<shape_t, shape_t::size>>::type;
};

template <typename shape_t, std::size_t I>
using to_multi_t = typename to_multi<shape_t, I>::type;

template <typename shape_t, typename idx_t>
constexpr auto to_multi_arg(idx_t&& idx){
    return to_multi_t<shape_t, idx_t::value>{};
}

}

}

#endif