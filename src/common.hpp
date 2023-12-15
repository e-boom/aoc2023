#pragma once

template <typename E>
constexpr std::underlying_type_t<E> to_underlying(E e) noexcept {
    return static_cast<std::underlying_type_t<E>>(e);
}

namespace AdventOfCode
{
    enum class Part
    {
        one,
        two
    };
}