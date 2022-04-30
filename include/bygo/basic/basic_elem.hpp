#ifndef BYGO_BASIC_ELEM_HPP
#define BYGO_BASIC_ELEM_HPP

#include <array>

#include <bygo/prop/shape.hpp>
#include <bygo/op/add.hpp>
#include <bygo/util/util.hpp>
#include <bygo/aux/aux.hpp>

namespace bygo{

template <typename shape_t, typename scalar_t = util::default_scalar_t, auto use_stl=false>
class basic_elem{
public:
    
    using data_t = std::conditional_t<use_stl, aux::make_storage_t<scalar_t, shape_t>, aux::make_storage_basic_t<scalar_t, shape_t>>;
    using shape_type = shape_t;
    using scalar_type = scalar_t;

    static constexpr auto nelem{shape_t::nelem};

    constexpr inline auto operator[](std::size_t i) const{
        return data_[i];
    }

    constexpr inline auto& operator[](std::size_t i){
        return data_[i];
    }

    /**
     *  @brief Element getter using parentheses
     */ 
public:
    template <typename _Ax, typename ..._Axs>
    constexpr inline decltype(auto) operator()(_Ax _ax, _Axs... _axs) const{

        static_assert(sizeof...(_axs) + 1 <= shape_t::size, "[bygo::basic_elem] Number of axis is too much.");

        using Is = std::make_index_sequence<sizeof...(_Axs)+1>;

        return this->at(std::forward_as_tuple(_ax, _axs...), Is{});
    }

    template <typename _Ax, typename = std::enable_if_t<util::is_integral_constant_v<_Ax>>>
    constexpr inline decltype(auto) operator()(_Ax&& _ax) const{

        static_assert((0 <= _ax) & (_ax < shape_t::nelem), "[bygo::basic_elem] Index out of bounds.");

        using Is = aux::to_multi_t<shape_type, _Ax::value>;
        
        return this->tup_to_at(Is{});
    }

private:
    template <std::size_t ...I>
    constexpr inline auto tup_to_at(std::index_sequence<I...>) const{
        using Is = std::make_index_sequence<sizeof...(I)>;

        return this->at(std::forward_as_tuple(I...), Is{});
    }

    template <typename tup_t, std::size_t ...I>
    constexpr inline auto at(tup_t&& _tup, std::index_sequence<I...>) const{

        return this->reverse_at(std::get<(sizeof...(I)-1)-I>(_tup)...);
    }

    template <typename _Ax, typename ..._Axs>
    constexpr inline auto reverse_at(_Ax _ax, _Axs... _axs) const{

        return this->reverse_at(_axs...)[_ax];
    }

    template <typename _Ax>
    constexpr inline auto reverse_at(_Ax _ax) const{

        return (*this)[_ax];
    }

    /**
     *  @brief Element setter using parentheses
     */ 
public:
    template <typename _Ax, typename ..._Axs>
    constexpr inline decltype(auto) operator()(_Ax _ax, _Axs... _axs){

        static_assert(sizeof...(_axs) + 1 <= shape_t::size, "[bygo::basic_elem] Number of axis is too much.");

        using Is = std::make_index_sequence<sizeof...(_Axs)+1>;

        return this->at(std::forward_as_tuple(_ax, _axs...), Is{});
    }

    template <typename _Ax, typename = std::enable_if_t<util::is_integral_constant_v<_Ax>>>
    constexpr inline decltype(auto) operator()(_Ax&& _ax){

        static_assert((0 <= _ax) & (_ax < shape_t::nelem), "[bygo::basic_elem] Index out of bounds.");

        using Is = aux::to_multi_t<shape_type, _Ax::value>;

        return this->tup_to_at(Is{});
    }

private:
    template <std::size_t ...I>
    constexpr inline auto& tup_to_at(std::index_sequence<I...>){

        using Is = std::make_index_sequence<sizeof...(I)>;

        return this->at(std::forward_as_tuple(I...), Is{});
    }

    template <typename tup_t, std::size_t ...I>
    constexpr inline auto& at(tup_t&& _tup, std::index_sequence<I...>){

        return this->reverse_at(std::get<(sizeof...(I)-1)-I>(_tup)...);
    }

    template <typename _Ax, typename ..._Axs>
    constexpr inline auto& reverse_at(_Ax _ax, _Axs... _axs){

        return this->reverse_at(_axs...)[_ax];
    }

    template <typename _Ax>
    constexpr inline auto& reverse_at(_Ax _ax){

        return (*this)[_ax];
    }
    
public:
    template <typename op_t>
    constexpr auto operator+(op_t&& _op){

        static_assert(aux::is_shape_equal_v<shape_type, typename util::remove_cvref_t<op_t>::shape_type>, "[bygo::basic_elem] Shape must be the same!");

        return op::add((*this), std::forward<op_t>(_op));
    }

    data_t data_;
};

namespace aux{

/**
 *  @brief Compare two basic_elem whether it is equal (in terms of scalar type and its elements) or not
 */ 
namespace impl{

    template <typename shape_t, typename obj1_t, typename obj2_t, std::size_t... I, typename ...axes_t>
    constexpr auto _is_elem_wise_equal(obj1_t&& obj1, obj2_t&& obj2, std::index_sequence<I...>, axes_t... axes){

        if(aux::is_scalar_v<util::remove_cvref_t<decltype(obj1(axes...))>>){

            return (obj1(axes...) == obj2(axes...));
        }else{

            using Is = std::make_index_sequence<shape_t::dim>;

            return (_is_elem_wise_equal<typename shape_t::res_shape>(std::forward<obj1_t>(obj1), std::forward<obj2_t>(obj2), Is{}, axes..., I) && ...);
        }
    }

    template <typename shape_t, typename obj1_t, typename obj2_t, std::size_t... I>
    constexpr auto is_elem_wise_equal(obj1_t&& obj1, obj2_t&& obj2, std::index_sequence<I...>){

        using Is = std::make_index_sequence<shape_t::dim>;

        return (_is_elem_wise_equal<typename shape_t::res_shape>(std::forward<obj1_t>(obj1), std::forward<obj2_t>(obj2), Is{}, I) && ...);
    }
}

template <typename obj1_t, typename obj2_t>
constexpr auto is_equal(obj1_t&& obj1, obj2_t&& obj2){

    // Simultaneously check the size and its scalar type
    if(std::is_same_v<util::remove_cvref_t<obj1_t>, util::remove_cvref_t<obj2_t>>){

        // If the type is not equal, we don't need to evaluate this
        using shape_type = typename util::remove_cvref_t<obj1_t>::shape_type;
        using Is = std::make_index_sequence<shape_type::dim>;

        return impl::is_elem_wise_equal<typename shape_type::res_shape>(std::forward<obj1_t>(obj1), std::forward<obj2_t>(obj2), Is{});
    }else{

        return false;
    }
}

/**
 *  @brief Compare two basic_elem whether it is approximate each other or not
 */ 
namespace impl{

    template <typename shape_t, typename obj1_t, typename obj2_t, typename tol_t, std::size_t... I, typename ...axes_t>
    constexpr auto _is_elem_wise_approx(obj1_t&& obj1, obj2_t&& obj2, tol_t tol, std::index_sequence<I...>, axes_t... axes){

        if(aux::is_scalar_v<util::remove_cvref_t<decltype(obj1(axes...))>>){

            return abs(obj1(axes...) - obj2(axes...)) < tol;
        }else{

            using Is = std::make_index_sequence<shape_t::dim>;

            return (_is_elem_wise_approx<typename shape_t::res_shape>(std::forward<obj1_t>(obj1), std::forward<obj2_t>(obj2), tol, Is{}, axes..., I) && ...);
        }
    }

    template <typename shape_t, typename obj1_t, typename obj2_t, typename tol_t, std::size_t... I>
    constexpr auto is_elem_wise_approx(obj1_t&& obj1, obj2_t&& obj2, tol_t tol, std::index_sequence<I...>){

        using Is = std::make_index_sequence<shape_t::dim>;

        return (_is_elem_wise_approx<typename shape_t::res_shape>(std::forward<obj1_t>(obj1), std::forward<obj2_t>(obj2), tol, Is{}, I) && ...);
    }
}

template <typename obj1_t, typename obj2_t, typename tol_t>
constexpr auto is_approx(obj1_t&& obj1, obj2_t&& obj2, tol_t tol){

    // Simultaneously check the size and its scalar type
    if(std::is_same_v<util::remove_cvref_t<obj1_t>, util::remove_cvref_t<obj2_t>>){

        // If the type is not equal, we don't need to evaluate this
        using shape_type = typename util::remove_cvref_t<obj1_t>::shape_type;
        using Is = std::make_index_sequence<shape_type::dim>;

        return impl::is_elem_wise_approx<typename shape_type::res_shape>(std::forward<obj1_t>(obj1), std::forward<obj2_t>(obj2), tol, Is{});
    }else{

        return false;
    }
}

}

}

#endif