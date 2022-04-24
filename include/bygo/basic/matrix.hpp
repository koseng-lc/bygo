#ifndef BYGO_BASIC_MATRIX_HPP
#define BYGO_BASIC_MATRIX_HPP

#include <bygo/basic/basic_elem.hpp>
#include <bygo/prop/shape.hpp>

namespace bygo{

template <std::size_t rows, std::size_t cols, typename scalar_t = util::default_scalar_t, typename shape_t = shape<rows, cols>>
class matrix: public basic_elem<shape_t, scalar_t>{
public:
    using transpose_type = matrix<cols, rows, scalar_t>;

    static constexpr auto nrows{rows};
    static constexpr auto ncols{cols};

    constexpr matrix(){

    }

    constexpr matrix(basic_elem<shape_t, scalar_t> _data)
        : basic_elem<shape_t, scalar_t>(_data){

    }

};

}

#endif