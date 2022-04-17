#ifndef BYGO_OP_FILL_HPP
#define BYGO_OP_FILL_HPP

#include <bygo/op/apply.hpp>
#include <bygo/prop/shape.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

namespace impl{
    struct fill{
        template <typename in_t, typename op_t>
        constexpr auto operator()(in_t&& in, op_t&& op) -> std::common_type_t<in_t, op_t>{
            (void)in;
            return op;
        }
    };
}

template <typename in_t, typename op_t>
constexpr auto fill(in_t&& in, op_t&& op){
    apply<typename util::remove_cvref_t<in_t>::shape_type>(impl::fill(), in, op, in);
}

} // namespace bygo::op

#endif