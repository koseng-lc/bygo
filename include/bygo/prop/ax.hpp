#ifndef BYGO_PROP_AX_HPP
#define BYGO_PROP_AX_HPP

namespace bygo{

    // template <std::size_t ...Axs>
    // struct ax{
    //     using type = std::index_sequence<Axs...>;
    // };

    template <typename Ax, typename ...Axs>
    constexpr auto ax(Ax ax, Axs... axs){
        return std::index_sequence<ax, axs...>{};
    }

    // template <std::size_t ...Axs>
    // using ax_t = ax<Axs...>::type;

} // namespace bygo

#endif