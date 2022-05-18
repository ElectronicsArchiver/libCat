// -*- mode: c++ -*-
// vim: set ft=cpp:
#pragma once

#include <cat/bit>

// Returns a value rounded up from `p_value` to the nearest `alignment`
// boundary.
template <typename U>
constexpr auto cat::align_up(U* const p_value, usize const alignment) -> U* {
    return (uintptr<U>{p_value} + (alignment - 1u)) & (~(alignment - 1u));
}

// Returns a value rounded up from `p_value` to the nearest `alignment`
// boundary.
template <typename U>
constexpr auto cat::align_up(uintptr<U> const p_value, usize const alignment)
    -> uintptr<U> {
    return (p_value + (alignment - 1u)) & (~(alignment - 1u));
}
