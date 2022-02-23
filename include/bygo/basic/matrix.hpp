#ifndef BYGO_MATRIX_HPP
#define BYGO_MATRIX_HPP

#include <bygo/basic/basic_elem.hpp>
#include <bygo/prop/shape.hpp>

namespace bygo{

template <typename scalar_t, typename shape_t>
class matrix: basic_elem<scalar_t, shape_t>{
    static constexpr auto nrows{shape_t::dim};
    static constexpr auto ncols{shape_t::res_shape::dim};
};

}

#endif