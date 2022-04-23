#ifndef BYGO_OP_SLICE_HPP
#define BYGO_OP_SLICE_HPP

#include <bygo/basic/basic_elem.hpp>

#define BYGO_IVAL(...) \
    [](){return std::pair{__VA_ARGS__};}

namespace bygo::op{

namespace impl{

    constexpr auto test(){
        return 1;
    }

    template <typename arg_t>
    constexpr auto lb(arg_t&& arg){
        return std::get<0>(arg);
    }

    template <typename arg_t>
    constexpr auto ub(arg_t&& arg){
        return std::get<1>(arg);
    }

    template <typename ival_t>
    constexpr auto d(ival_t&& ival){
        return ub(ival) - lb(ival);
    }

    template <typename ival_t, typename ...ivals_t, std::size_t... I>
    constexpr auto deduce_shape(ival_t&& ival, ivals_t&&... ivals, std::index_sequence<I...>){
        return ::bygo::shape<d(ival()) + 1, (d(ivals()) + 1)...>{};
    }
}

template <typename in_t, typename ival_t, typename ...ivals_t>
constexpr auto slice(in_t&& in, ival_t&& ival, ivals_t&&... ivals){
    using in_type = util::remove_cvref_t<in_t>;
    using out_shape = decltype(impl::deduce_shape(std::forward<ival_t>(ival), std::forward<ivals_t>(ivals)..., std::make_index_sequence<sizeof...(ivals_t)>{}));
    using out_type = bygo::basic_elem<typename in_type::shape_type, out_shape>;

    out_type res{};

    return res;
}

}

#endif