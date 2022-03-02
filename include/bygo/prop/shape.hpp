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

}

}

#endif