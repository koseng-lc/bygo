#ifndef BYGO_OP_RREF_HPP
#define BYGO_OP_RREF_HPP

#include <bygo/op/add.hpp>
#include <bygo/op/mult.hpp>
#include <bygo/op/div.hpp>
#include <bygo/op/dot.hpp>
#include <bygo/op/swap_elem.hpp>
#include <bygo/util/util.hpp>

namespace bygo::op{

namespace impl{

    template <typename in_t>
    constexpr auto find_nonzero_pos(in_t&& in, auto& index, auto& lead, const auto& row){
        if(in[index][lead] == 0){
            index += 1;

            using in_type = util::remove_cvref_t<in_t>;

            constexpr auto nrows{aux::nth_shape_dim_v<typename in_type::shape_type, 1>};
            constexpr auto ncols{aux::nth_shape_dim_v<typename in_type::shape_type, 2>};

            if(index >= nrows){
                index = row;
                lead += 1;
                if(lead >= ncols){
                    return 1;
                }
            }

            return find_nonzero_pos(std::forward<in_t>(in), index, lead, row);
        }

        return 0;
    }

    struct rref_sub2{
        template <typename in_t, typename lead_t, typename row_t>
        constexpr auto operator()(in_t&& in, lead_t&& lead, row_t&& row, std::size_t i){
            if(i != row){
                auto k(in(i, lead));
                auto multiplied_in(::bygo::op::mult(std::forward<in_t>(in), -k, std::make_tuple(row)));
                in = ::bygo::op::add(std::forward<in_t>(in), multiplied_in, std::make_tuple(i), std::make_tuple(row));
            }
        }
    };

    struct rref_sub1{
        template <typename in_t, typename out_t, typename lead_t>
        constexpr auto operator()(in_t&& in, out_t&& out, lead_t&& lead, std::size_t row){
            
            using out_type = util::remove_cvref_t<out_t>;

            constexpr auto nrows{aux::nth_shape_dim_v<typename out_type::shape_type, 1>};
            constexpr auto ncols{aux::nth_shape_dim_v<typename out_type::shape_type, 2>};

            auto index(row);
            if(lead >= ncols){
                return;
            }

            if(find_nonzero_pos(std::forward<out_t>(out), index, lead, row)){
                return;
            }
            
            out = ::bygo::op::swap_elem(std::forward<out_t>(out), std::make_tuple(index), std::make_tuple(row));
            out = ::bygo::op::div(std::forward<out_t>(out), out(row, lead), std::make_tuple(row));

            util::for_loop<nrows>(rref_sub2(), std::forward<out_t>(out), std::forward<lead_t>(lead), std::forward<std::size_t>(row));
        }
    };

    template <typename in_t, typename out_t>
    constexpr auto rref(in_t&& in, out_t&& out){
        std::size_t lead(0);

        constexpr auto nrows{aux::nth_shape_dim_v<typename util::remove_cvref_t<out_t>::shape_type, 1>};

        util::for_loop<nrows>(rref_sub1(), std::forward<in_t>(in), std::forward<out_t>(out), std::forward<std::size_t>(lead));
    }
}

template <typename in_t>
constexpr auto rref(in_t&& in){
    using out_type = util::remove_cvref_t<in_t>;

    out_type res(in);

    impl::rref(std::forward<in_t>(in), res);

    return res;
}

}

#endif