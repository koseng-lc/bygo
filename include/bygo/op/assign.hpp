#ifndef BYGO_OP_ASSIGN_HPP
#define BYGO_OP_ASSIGN_HPP

#include <bygo/op/apply.hpp>
#include <bygo/aux/aux.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

namespace impl{
    
    struct _assign{
        template <typename in_t, typename op_t>
        constexpr auto operator()(in_t in, op_t op) -> std::common_type_t<util::remove_cvref_t<in_t>, util::remove_cvref_t<op_t>>{
            return op;
        }
    };

    template <typename in_t, typename op_t, typename out_t, typename axes1_t, typename axes2_t>
    constexpr auto assign(in_t&& in, op_t&& op, out_t&& out, axes1_t&& axes1, axes2_t&& axes2){
        
        ::bygo::op::apply(_assign(), std::forward<in_t>(in), std::forward<op_t>(op), std::forward<out_t>(out)
            , std::forward<axes1_t>(axes1), std::forward<axes2_t>(axes2));
    }
} // namespace impl

template <typename in_t, typename op_t, typename axes1_t = whole_axes_t, typename axes2_t = whole_axes_t>
constexpr auto assign(in_t&& in, op_t&& op, axes1_t&& axes1 = whole_axes_t{}, axes2_t&& axes2 = whole_axes_t{}){

    using out_type = util::remove_cvref_t<in_t>;

    out_type res(in);
    impl::assign(std::forward<in_t>(in), std::forward<op_t>(op), res, std::forward<axes1_t>(axes1), std::forward<axes2_t>(axes2));

    return res;
}

} // namespace bygo::op

#endif