#ifndef BYGO_BASIC_MATRIX_HPP
#define BYGO_BASIC_MATRIX_HPP

#include <bygo/basic/basic_elem.hpp>
#include <bygo/prop/shape.hpp>

namespace bygo{

template <typename scalar_t, std::size_t rows, std::size_t cols, typename _shape_t = shape<rows, cols>>
class matrix: public basic_elem<scalar_t, _shape_t>{
public:
    using shape_t = _shape_t;

    static constexpr auto nrows{rows};
    static constexpr auto ncols{cols};

    // matrix(basic_elem<scalar_t, _shape_t> _data)
    //     : data_(_data){

    // }



};

}

#endif