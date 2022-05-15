#ifndef BYGO_CTYPE_EYE_HPP
#define BYGO_CTYPE_EYE_HPP

#include <bygo/basic/matrix.hpp>
#include <bygo/op/fill.hpp>
#include <bygo/aux/aux.hpp>
#include <bygo/util/util.hpp>

namespace bygo::ctype{

namespace impl{

    template <typename out_t, std::size_t ...J>
    constexpr auto _eye(out_t&& out, std::size_t i, std::index_sequence<J...>){

        using out_type = util::remove_cvref_t<out_t>;
        using scalar_type = typename out_type::scalar_type;

        ((out(i, J) = (scalar_type)(i == J)), ...);
    }

    template <std::size_t NC, typename out_t, std::size_t ...I>
    constexpr auto eye(out_t&& out, std::index_sequence<I...>){

        using Js = std::make_index_sequence<NC>;

        (_eye(std::forward<out_t>(out), I, Js{}), ...);
    }
}

template <std::size_t NR, std::size_t NC, typename scalar_t = util::default_scalar_t>
constexpr auto eye(){

    using out_type = basic::matrix<NR, NC, scalar_t>;
    using Is = std::make_index_sequence<NR>;

    out_type res{};
    impl::eye<NC>(res, Is{});

    return res;
} // namespace impl

template <typename in_t>
constexpr auto eye(in_t&& in){
    
    using out_type = util::remove_cvref_t<in_t>;
    using Is = std::make_index_sequence<out_type::nrows>;

    out_type res(in);
    impl::eye<out_type::ncols>(res, Is{});

    return res;
}

} // namespace bygo::ctype

#endif