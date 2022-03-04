#ifndef BYGO_OP_INV_HPP
#define BYGO_OP_INV_HPP

#include <bygo/op/dot.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

namespace impl{
    // template <typename in_t, typename out_t>
    // constexpr auto rref_sub1(in_t&& in, out_t&& out, std::size_t idx){
    //     // ( , ...);
    // }

    template <typename in_t, typename out_t>
    constexpr auto find_nonzero_pos(in_t&& in, out_t&& out){
        
    }

    struct rref_sub1{
        template <typename in_t, typename out_t, typename lead_t>
        constexpr auto operator()(in_t&& in, out_t&& out, lead_t&& lead, std::size_t idx){
            // std::cout << "WOYY" << std::endl;
            in[idx];
            out[idx];
            using out_type = util::remove_cvref_t<out_t>;

            constexpr auto nrows{out_type::nrows};
            constexpr auto ncols{out_type::ncols};

            auto index(idx);
            if(lead > ncols){
                return;
            }
        }
    };

    template <typename in_t, typename out_t, std::size_t ...I>
    constexpr auto rref(in_t&& in, out_t&& out,  std::index_sequence<I...>){
        std::size_t lead(0);
        util::for_loop<sizeof...(I)>(rref_sub1(), std::forward<in_t>(in), std::forward<out_t>(out), std::forward<std::size_t>(lead));
    }

    template <typename in_t, typename out_t,
            typename Is = std::make_index_sequence<util::remove_cvref_t<out_t>::nrows>>
    constexpr auto inv(in_t&& in, out_t&& out){
        rref(std::forward<in_t>(in), std::forward<out_t>(out), Is{});
    }
}

template <typename in_t>
constexpr auto inv(in_t&& in){
    using out_t = util::remove_cvref_t<in_t>;

    out_t res;

    impl::inv(std::forward<in_t>(in), res);

    return res;
}

}

#endif