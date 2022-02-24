#ifndef BYGO_UTIL_HPP
#define BYGO_UTIL_HPP

namespace bygo::util{
    template <typename T>
    struct remove_cvref{
        using type = std::remove_cv_t<std::remove_reference_t<T>>;
    };

    template <typename T>
    using remove_cvref_t = typename remove_cvref<T>::type;
    
} // namespace bygo::util

#endif