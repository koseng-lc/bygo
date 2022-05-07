#ifndef BYGO_PROP_AX_HPP
#define BYGO_PROP_AX_HPP

#include <bygo/util/util.hpp>

namespace bygo{

using whole_axes_t = std::tuple<>;

template <typename axes_t>
struct is_whole_axes{
    using axes_type = util::remove_cvref_t<axes_t>;
    static constexpr auto value = std::is_same<axes_type, whole_axes_t>::value;
};

template <typename axes_t>
static constexpr auto is_whole_axes_v = is_whole_axes<axes_t>::value;

constexpr auto whole_ax(){
    return whole_axes_t{};
}

template <typename ...Axs>
constexpr auto ax(Axs ...axs){
    
    return std::make_tuple(axs...);
}

} // namespace bygo

#endif