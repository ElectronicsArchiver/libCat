#include <cat/string>

auto main() -> int {
    char const* p_string_1 = "Hello!";
    char const* const p_string_2 = "Hello!";

    cat::String string_1 = "Hello!";
    cat::String const string_2 = "Hello!";
    cat::String string_3 = "Goodbye!";

    cat::String long_string_1 =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    cat::String long_string_2 =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

    // Test a succesful string pointer case.
    if (!cat::compare_strings(p_string_1, p_string_2)) {
        cat::exit(1);
    }

    // Test a succesful string view case.
    if (!cat::compare_strings(string_1, string_2)) {
        cat::exit(1);
    }

    // Test a succesful large string case.
    if (!cat::compare_strings(long_string_1, long_string_2)) {
        // TODO: Fix this failing test!
        cat::exit(1);
    }

    // Test a failure case.
    if (cat::compare_strings(string_1, string_3)) {
        cat::exit(1);
    }

    [[maybe_unused]] cat::String const_string_1 = "Hello, ";
    [[maybe_unused]] constexpr cat::String const_string_2 = "world!";

    // Fixed length strings.
    constexpr cat::StaticString const_string_3 = "Hello, ";
    constexpr cat::StaticString const_string_4 = "world!";

    // Test collection operations.
    _ = const_string_1[1];
    Result(!const_string_3.at(10).has_value()).or_exit();

    // TODO: Make this `constexpr`.
    cat::StaticString hello_world = (const_string_3 + const_string_4);
    constexpr cat::StaticString const_hello_world =
        (const_string_3 + const_string_4);
    Result(cat::compare_strings(hello_world, "Hello, world!")).or_exit();
    Result(cat::compare_strings(const_hello_world, "Hello, world!")).or_exit();

    ssize const h = const_string_1.find('H').value();
    ssize const e = const_string_1.find('e').value();
    ssize const l = const_string_1.find('l').value();
    ssize const o = const_string_1.find('o').value();
    Result(h == 0).or_exit();
    Result(e == 1).or_exit();
    Result(l == 2).or_exit();
    Result(o == 4).or_exit();
}
