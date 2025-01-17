#include <cat/array>
#include <cat/linear_allocator>
#include <cat/math>
#include <cat/numerals>
#include <cat/page_allocator>
#include <cat/utility>

auto main() -> int {
    // Initialize an allocator.
    cat::PageAllocator paging_allocator;

    int4* p_page = paging_allocator.p_alloc_multi<int4>(4_ki).or_exit();
    defer(paging_allocator.free_multi(p_page, 4_ki);)
    cat::LinearAllocator allocator(p_page, 24);
    // It should not be possible to allocate 7 times here, because 24 bytes can
    // only hold 6 `int4`s.
    for (int i = 0; i < 7; ++i) {
        cat::Optional handle = allocator.alloc<int4>();
        if (!handle.has_value()) {
            Result(i == 6).or_exit();
            goto overallocated;
        }
    }
    cat::exit(1);

overallocated:
    // Invalidate all memory handles, and allocate again.
    allocator.reset();
    for (int4 i = 0; i < 4; ++i) {
        cat::Optional handle = allocator.alloc<cat::Byte>();
        Result(handle.has_value()).or_exit();
    }
    // This allocated 16 bytes, which is 8-byte-aligned. Another int allocation
    // would make it 4-byte-aligned. However, 8 bytes should be allocated here
    // to keep it 8-byte-aligned.
    auto handle = allocator.align_alloc<int4>(8u).value();
    Result(cat::is_aligned(&allocator.get(handle), 8u)).or_exit();

    // Allocate another int.
    auto handle_2 = allocator.alloc<int4>().value();
    Result(cat::is_aligned(&allocator.get(handle_2), 4u)).or_exit();
    // This is now 4-byte-aligned.
    Result(!cat::is_aligned(&allocator.get(handle_2), 8u)).or_exit();

    // Small size allocations shouldn't bump the allocator.
    for (int4 i = 0; i < 20; ++i) {
        auto memory = allocator.inline_alloc<int4>();
        Result(memory.has_value()).or_exit();
    }
    cat::Optional handle_3 = allocator.alloc<int4>();
    Result(handle_3.has_value()).or_exit();

    // Test that allocations are reusable.
    allocator.reset();
    decltype(allocator.alloc<int1>()) handles[4];
    for (signed char i = 0; i < 4; ++i) {
        handles[i] = allocator.alloc<int1>();
        Result(handles[i].has_value()).or_exit();
        allocator.get(handles[i].value()) = i;
    }
    for (signed char i = 0; i < 4; ++i) {
        Result(allocator.get(handles[i].value()) == i).or_exit();
    }

    // Test that allocations have pointer stability.
    allocator.reset();
    int4* p_handles[4];
    for (int i = 0; i < 4; ++i) {
        int4* p_handle = allocator.p_alloc<int4>().or_exit();
        *p_handle = i;
        p_handles[i] = p_handle;
    }
    for (int i = 0; i < 4; ++i) {
        Result(*(p_handles[i]) == i).or_exit();
        allocator.free(p_handles[i]);
    }

    allocator.reset();
    // Test allocation constructors.
    int4* p_initialized = allocator.p_alloc<int4>(100).or_exit();
    Result(*p_initialized == 100).or_exit();

    // Test sized allocations.
    allocator.reset();
    _ = allocator.alloc<int2>().or_exit();
    // Because the allocator is now 2 byte aligned, an extra 2 bytes have to be
    // reserved to allocate a 4-byte aligned value:
    Result(allocator.nalloc<int4>().or_exit() == 6).or_exit();
    cat::Tuple alloc_int_size = allocator.salloc<int4>().value();
    Result(alloc_int_size.second() == 6).or_exit();

    // TODO: Test multi allocations.
    // TODO: Test inline multi allocations.
}
