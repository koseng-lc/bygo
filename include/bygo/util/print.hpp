#ifndef BYGO_UTIL_PRINT_HPP
#define BYGO_UTIL_PRINT_HPP

#include <bygo/util/util.hpp>

namespace bygo::util{

template <typename in_t>
auto print_matrix(in_t&& in){
    using in_type = util::remove_cvref_t<in_t>;
    for(auto i(0); i < in_type::nrows; i++){
        for(auto j(0); j < in_type::ncols; j++){
            std::cout << in(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

} // namespace bygo::util

#endif