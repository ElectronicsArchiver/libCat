#include <cat/simd>

// TODO: Document.
auto is_avx512f_supported() -> bool1 {
    __builtin_cpu_init();
    return __builtin_cpu_supports("avx512f");
}
