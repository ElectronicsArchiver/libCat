#include <type_list>
#include <variant>

void meow() {
    using Types = meta::TypeList<int, char>;
    using Type_0 = typename Types::Get<0>;
    using Type_1 = typename Types::Get<1>;
    static_assert(meta::is_same<Type_0, int>);
    static_assert(meta::is_same<Type_1, char>);

    Variant<int, char, uint4> variant(int{1});
    Result(variant.is_type<int>()).or_panic();
    int foo_int = variant.value<int>();
    Result(foo_int == 1).or_panic();

    static_assert(variant.type_index<int>() == 0);
    static_assert(variant.type_index<char>() == 1);
    static_assert(variant.type_index<uint4>() == 2);

    variant = 'o';
    Result(variant.is_type<char>()).or_panic();
    char foo_char = variant.value<char>();
    Result(foo_char == 'o').or_panic();

    cat::exit();
}