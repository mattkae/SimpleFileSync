#pragma once
#include <type_traits>

namespace shared {
    template <typename E>
    constexpr auto enumToUnderlying(E e) noexcept {
        return static_cast<std::underlying_type_t<E>>(e);
    }
}