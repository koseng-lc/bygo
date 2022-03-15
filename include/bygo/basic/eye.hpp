#ifndef BYGO_BASIC_EYE_HPP
#define BYGO_BASIC_EYE_HPP

#include <bygo/basic/matrix.hpp>
#include <bygo/ctype/eye.hpp>

namespace bygo{

template <typename scalar_t, std::size_t rows, std::size_t cols>
class eye: public matrix<scalar_t, rows, cols>{
public:
    constexpr eye(){

    }
};

}

#endif