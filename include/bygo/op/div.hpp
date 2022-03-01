#ifndef BYGO_OP_DIV_HPP
#define BYGO_OP_DIV_HPP

#include <bygo/op/apply.hpp>
#include <bygo/prop/shape.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

namespace impl{
    struct div{
        template <typename in_t, typename op_t>
        constexpr auto operator()(in_t&& in, op_t&& op) -> std::common_type_t<in_t, op_t>{
            return in / op;
        }
    };
}

template <typename in_t, typename op_t, typename out_t>
constexpr auto div(in_t&& in, op_t&& op, out_t&& out){
    apply(impl::div(), in, op, out);
}

} // namespace bygo::op

#endif