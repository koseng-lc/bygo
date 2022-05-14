#ifndef BYGO_HELPER_HPP
#define BYGO_HELPER_HPP

namespace bygo::aux{

/**
 *  @brief using std::array 
 */ 
template <std::size_t _s, typename _elem_t, typename _shape_t>
struct make_storage{
    using type = std::array<typename make_storage<_s-1, _elem_t, typename _shape_t::res_shape>::type, _shape_t::dim>;
};

template <typename _elem_t, typename _shape_t>
struct make_storage<1, _elem_t, _shape_t>{
    using type = std::array<_elem_t, _shape_t::dim>;
};

template <typename _elem_t, typename _shape_t>
using make_storage_t = typename make_storage<_shape_t::size, _elem_t, _shape_t>::type;

/**
 *  @brief using basic array
 */ 
template <std::size_t _s, typename _elem_t, typename _shape_t>
struct make_storage_basic;

template <std::size_t _s, typename _elem_t, typename _shape_t>
struct make_storage_basic{
    using array_type = typename make_storage_basic<_s-1, _elem_t, typename _shape_t::res_shape>::type;
    using type = array_type[_shape_t::dim];
};

template <typename _elem_t, typename _shape_t>
struct make_storage_basic<1, _elem_t, _shape_t>{
    using type = _elem_t[_shape_t::dim];
};

template <typename _elem_t, typename _shape_t>
using make_storage_basic_t = typename make_storage_basic<_shape_t::size, _elem_t, _shape_t>::type;

/**
 *  @brief count the number of elements in basic_elem
 */ 
template <std::size_t ..._ds>
struct nelem{
    static constexpr auto value = 1;
};

template <std::size_t _d, std::size_t ..._ds>
struct nelem<_d, _ds...>{
    static constexpr auto value = _d*nelem<_ds...>::value;
};

template <std::size_t _d, std::size_t ..._ds>
static constexpr auto nelem_v = nelem<_d, _ds...>::value;

/**
 *  @brief check whether the type is scalar or not
 */ 
template <typename T>
struct is_scalar{
    static constexpr auto value = std::is_integral<T>::value | std::is_floating_point<T>::value;
};

template <typename T>
static constexpr auto is_scalar_v = is_scalar<T>::value;

/**
 *  @brief check whether the type is tuple or not
 *  @tparam the type being checked
 */ 
template <typename T>
struct is_tuple : std::false_type {};

template <typename ...Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type {};

template <typename T>
static constexpr auto is_tuple_v = is_tuple<T>::value;

/**
 *  @brief check whether the type has shape_type or not
 *  @tparam the type being checked
 */ 
template <typename T, typename = void>
struct has_shape : std::false_type {};

template <typename T>
struct has_shape<T, std::void_t<typename T::shape_type>> : std::true_type {};

template <typename T>
static constexpr auto has_shape_v = has_shape<T>::value;

} // namespace bygo::aux

#endif