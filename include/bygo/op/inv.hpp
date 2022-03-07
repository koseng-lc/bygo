#ifndef BYGO_OP_INV_HPP
#define BYGO_OP_INV_HPP

#include <bygo/op/add.hpp>
#include <bygo/op/mult.hpp>
#include <bygo/op/div.hpp>
#include <bygo/op/dot.hpp>
#include <bygo/op/swap_elem.hpp>
#include <bygo/util/util.hpp>
#include <bygo/util/print.hpp>

namespace bygo::op{

namespace impl{

    template <typename in_t>
    constexpr void find_nonzero_pos(in_t&& in, auto& index, auto& lead, const auto& row){
        if(in[index][lead] == 0){
            index += 1;

            using in_type = util::remove_cvref_t<in_t>;

            constexpr auto nrows{in_type::nrows};
            constexpr auto ncols{in_type::ncols};

            if(index >= nrows){
                index = row;
                lead += 1;
                if(lead >= ncols){
                    return;
                }
            }

            find_nonzero_pos(std::forward<in_t>(in), index, lead, row);
        }
    }

    struct rref_sub2{
        template <typename in_t, typename lead_t, typename row_t>
        constexpr auto operator()(in_t&& in, lead_t&& lead, row_t&& row, std::size_t i){
            if(i != row){
                auto k(in(i, lead));
                auto multiplied_in(::bygo::op::mult(std::forward<in_t>(in), -k, std::make_tuple(row)));
                // ::bygo::util::print_matrix(multiplied_in);
                // ::bygo::util::print_matrix(in);
                in = ::bygo::op::add(std::forward<in_t>(in), multiplied_in, std::make_tuple(i), std::make_tuple(row));
            }
        }
    };

    struct rref_sub1{
        template <typename in_t, typename out_t, typename lead_t>
        constexpr auto operator()(in_t&& in, out_t&& out, lead_t&& lead, std::size_t row){
            
            using out_type = util::remove_cvref_t<out_t>;

            constexpr auto nrows{out_type::nrows};
            constexpr auto ncols{out_type::ncols};

            auto index(row);
            if(lead >= ncols){
                return;
            }

            find_nonzero_pos(std::forward<out_t>(out), index, lead, row);
            
            out = ::bygo::op::swap_elem(std::forward<out_t>(out), std::make_tuple(index), std::make_tuple(row));

            util::for_loop<nrows>(rref_sub2(), std::forward<out_t>(out), std::forward<lead_t>(lead), std::forward<std::size_t>(row));

        }
    };

    template <typename in_t, typename out_t>
    constexpr auto rref(in_t&& in, out_t&& out){
        std::size_t lead(0);

        constexpr auto nrows{util::remove_cvref_t<out_t>::nrows};

        util::for_loop<nrows>(rref_sub1(), std::forward<in_t>(in), std::forward<out_t>(out), std::forward<std::size_t>(lead));
    }

    template <typename in_t, typename out_t>
    constexpr auto inv(in_t&& in, out_t&& out){
        rref(std::forward<in_t>(in), std::forward<out_t>(out));
    }
}

template <typename in_t>
constexpr auto inv(in_t&& in){
    using out_t = util::remove_cvref_t<in_t>;

    out_t res(in);
    
    impl::inv(std::forward<in_t>(in), res);

    return res;
}

}

#endif