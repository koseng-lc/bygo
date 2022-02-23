#ifndef BYGO_BASIC_ELEM_HPP
#define BYGO_BASIC_ELEM_HPP

#include <array>

#include <bygo/prop/shape.hpp>
#include <bygo/helper/helper.hpp>

namespace bygo{

template <typename scalar_t, typename shape_t, auto use_stl=false>
class basic_elem{
public:
    
    using data_t = std::conditional_t<use_stl, helper::make_storage_t<scalar_t, shape_t>, helper::make_storage_basic_t<scalar_t, shape_t>>;
    using shape = shape_t;

    static constexpr auto nelem{shape_t::nelem};

    constexpr inline auto operator[](std::size_t i) const{
        return data_[i];
    }

    constexpr inline auto& operator[](std::size_t i){
        return data_[i];
    }

    template <typename _Ax, typename ..._Axs>
    constexpr inline auto operator()(_Ax _ax, _Axs... _axs) const{
        static_assert(sizeof...(_axs) + 1 <= shape_t::size, "[basic_elem] Num of axis is too much.");
        return (*this)(_axs...)[_ax];
    }

    template <typename _Ax>
    constexpr inline auto operator()(_Ax _ax) const{
        return (*this)[_ax];
    }

    template <typename _Ax, typename ..._Axs>
    constexpr inline auto& operator()(_Ax _ax, _Axs... _axs){
        return (*this)(_axs...)[_ax];
    }

    template <typename _Ax>
    constexpr inline auto& operator()(_Ax _ax){
        return (*this)[_ax];
    }

    template <typename op_t>
    constexpr auto operator+(op_t _op){
        // basic_elem<scalar_t, shape_t> res;
        static_assert(helper::is_shape_equal_v<shape, typename op_t::shape>, "[basic_elem] Shape must be the same!");
        add_impl((*this), _op, (*this));
        return (*this);
    }

    template <typename A, typename B, typename C, typename _shape_t = shape, typename Is = std::make_index_sequence<_shape_t::dim>>
    constexpr auto add_impl(A&& a, B&& b, C&& c){
        if constexpr(helper::is_scalar_v<std::decay_t<A>>){
            c = a + b;
        }else{
            // add_impl<A, B, C, _shape_t>(a, b, c, Is{});
            add_impl<decltype(std::forward<decltype(a)>(a)), decltype(std::forward<decltype(b)>(b)), decltype(std::forward<decltype(c)>(c)), typename _shape_t::res_shape>
                (std::forward<decltype(a)>(a), std::forward<decltype(b)>(b), std::forward<decltype(c)>(c), Is{});

        }
    }

    template <typename A, typename B, typename C, typename _shape_t, std::size_t... I>
    constexpr auto add_impl(A&& a, B&& b, C&& c, std::index_sequence<I...>){
        (add_impl<decltype(std::forward<decltype(a[I])>(a[I])), decltype(std::forward<decltype(b[I])>(b[I])), decltype(std::forward<decltype(c[I])>(c[I])), typename _shape_t::res_shape>
            (std::forward<decltype(a[I])>(a[I]), std::forward<decltype(b[I])>(b[I]), std::forward<decltype(c[I])>(c[I])), ...);
    }

    //
    template <typename A, typename _elem_t, typename _shape_t = shape, typename Is = std::make_index_sequence<_shape_t::dim>>
    constexpr auto fill_impl(A&& a, _elem_t&& _val){
        // static_assert(helper::is_shape_equal_v<A::shape, B::shape>, "[basic_elem] Shape must be the same!");
        if constexpr(helper::is_scalar_v<std::decay_t<A>>){
            a = _val;
        }else{
            fill_impl<decltype(std::forward<decltype(a)>(a)), decltype(std::forward<decltype(_val)>(_val)), typename _shape_t::res_shape>
                (std::forward<decltype(a)>(a), std::forward<decltype(_val)>(_val), Is{});
        }
    }

    template <typename A, typename _elem_t, typename _shape_t, std::size_t... I>
    constexpr auto fill_impl(A&& a, _elem_t&& _val, std::index_sequence<I...>){
        (fill_impl<decltype(std::forward<decltype(a[I])>(a[I])), decltype(std::forward<decltype(_val)>(_val)), typename _shape_t::res_shape>
            (std::forward<decltype(a[I])>(a[I]), std::forward<decltype(_val)>(_val)), ...);
    }

    data_t data_;
};

// namespace basic_elem::impl{

// }

}

#endif