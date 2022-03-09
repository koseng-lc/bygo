#ifndef BYGO_OP_INSERT_HPP
#define BYGO_OP_INSERT_HPP

#include <bygo/aux/aux.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

namespace impl{

}

template <typename in_t, typename op_t, typename ...axes_t>
constexpr auto insert(in_t&& in, op_t&& op, std::tuple<axes_t...> axes){
    using in_type = util::remove_cvref_t<in_t>;
    using shape_type = aux::nth_shape_t<typename in_type::shape_type, sizeof...(axes_t)>;
    // using out_type = 
}

}

#endif