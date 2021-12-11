#pragma once

static auto simd_string_length(const char* p_string) -> i32 {
    i32 result = 0;
    u8x16* p_memory = reinterpret_cast<u8x16*>(const_cast<char*>(p_string));
    u8x16 zeros = simd_setzero<u8x16>();
    for (; (p_memory++) != nullptr; result += 16) {
        u8x16 data = simd_load(p_memory);
        constexpr u8 mask =
            ::SIDD_UBYTE_OPS | ::SIDD_CMP_EQUAL_EACH | ::SIDD_LEAST_SIGNIFICANT;
        if (simd_cmp_implicit_str_c<mask>(data, zeros)) {
            i32 const index = simd_cmp_implicit_str_i<mask>(data, zeros);
            return result + index;
        }
    }
    return 0;
}