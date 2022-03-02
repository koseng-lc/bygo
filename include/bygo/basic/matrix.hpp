#ifndef BYGO_BASIC_MATRIX_HPP
#define BYGO_BASIC_MATRIX_HPP

#include <bygo/basic/basic_elem.hpp>
#include <bygo/prop/shape.hpp>

namespace bygo{

template <typename scalar_t, std::size_t rows, std::size_t cols, typename shape_t = shape<rows, cols>>
class matrix: public basic_elem<scalar_t, shape_t>{
public:
    using transpose_type = matrix<scalar_t, cols, rows>;

    static constexpr auto nrows{rows};
    static constexpr auto ncols{cols};

    matrix(){

    }

    matrix(basic_elem<scalar_t, shape_t> _data)
        : basic_elem<scalar_t, shape_t>(_data){

    }

};

}

#endif