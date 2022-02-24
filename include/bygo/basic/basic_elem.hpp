#ifndef BYGO_BASIC_ELEM_HPP
#define BYGO_BASIC_ELEM_HPP

#include <array>

#include <bygo/prop/shape.hpp>
#include <bygo/op.hpp>
#include <bygo/util/util.hpp>
#include <bygo/helper/helper.hpp>

namespace bygo{

template <typename scalar_t, typename shape_t, auto use_stl=false>
class basic_elem{
public:
    
    using data_t = std::conditional_t<use_stl, helper::make_storage_t<scalar_t, shape_t>, helper::make_storage_basic_t<scalar_t, shape_t>>;
    using shape_type = shape_t;

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
        static_assert(helper::is_shape_equal_v<shape_type, typename op_t::shape_type>, "[basic_elem] Shape must be the same!");
        basic_elem<scalar_t, shape_t> res;
        op::add((*this), _op, res);
        return res;
    }

    data_t data_;
};

// namespace basic_elem::impl{

// }

}

#endif