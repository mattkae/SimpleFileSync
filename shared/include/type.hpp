#pragma once

#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <ctype.h>
#include <type_traits>

namespace shared {
	typedef unsigned char byte;
	typedef int8_t i8;
	typedef int32_t i32;
	typedef uint64_t u64;
	typedef uint32_t u32;

	template <typename E>
    constexpr auto getEnumType(E e) noexcept {
        return static_cast<std::underlying_type_t<E>>(e);
    }
}