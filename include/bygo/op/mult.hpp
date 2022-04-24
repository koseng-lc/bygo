#ifndef BYGO_OP_MULT_HPP
#define BYGO_OP_MULT_HPP

#include <bygo/op/apply.hpp>
#include <bygo/prop/shape.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

namespace impl{
    struct _mult{
        template <typename in_t, typename op_t>
        constexpr auto operator()(in_t&& in, op_t&& op) -> std::common_type_t<in_t, op_t>{
            return in * op;
        }
    };

    template <typename in_t, typename op_t, typename out_t, typename axes1_t, typename axes2_t, std::size_t ...I>
    constexpr auto mult(in_t&& in, op_t&& op, out_t&& out, axes1_t axes1, axes2_t axes2, std::index_sequence<I...>){

        if constexpr(aux::is_scalar_v<util::remove_cvref_t<op_t>>){
            ::bygo::op::apply(_mult(), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out), axes1);
        }else{
            ::bygo::op::apply(_mult(), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out), axes1, axes2);
        }
    }
}

template <typename in_t, typename op_t, typename ...axes1_t, typename ...axes2_t, typename Is = std::make_index_sequence<sizeof...(axes1_t)>>
constexpr auto mult(in_t&& in, op_t&& op, std::tuple<axes1_t...> axes1, std::tuple<axes2_t...> axes2){

    using out_type = util::remove_cvref_t<in_t>;

    out_type res(in);
    impl::mult(std::forward<in_t>(in), std::forward<op_t>(op), res, axes1, axes2, Is{});

    return res;
}

template <typename in_t, typename op_t, typename ...axes_t>
constexpr auto mult(in_t&& in, op_t&& op, std::tuple<axes_t...> axes){

    return mult(std::forward<in_t>(in), std::forward<op_t>(op), axes, axes);
}

template <typename in_t, typename op_t>
constexpr auto mult(in_t&& in, op_t&& op){
    
    using out_type = util::remove_cvref_t<in_t>;

    out_type res(in);
    apply<typename out_type::shape_type>(impl::_mult(), std::forward<in_t>(in), std::forward<op_t>(op), res);

    return res;
}

} // namespace bygo::op

#endif