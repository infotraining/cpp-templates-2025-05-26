#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std::literals;

namespace Ver_1
{
    template <typename T>
    T max_value(T a, T b)
    {
        return a < b ? b : a;
    }

    template <typename T>
        requires std::is_pointer_v<T>
    auto max_value(T a, T b)
    {
        assert(a != nullptr);
        assert(b != nullptr);
        return max_value(*a, *b);
    }
} // namespace Ver_1

namespace Ver_2
{
    auto max_value(auto a, auto b)
        requires std::is_pointer_v<decltype(a)> && std::is_pointer_v<decltype(a)> && std::same_as<decltype(a), decltype(b)>
    {
        assert(a != nullptr);
        assert(b != nullptr);
        return max_value(*a, *b);
    }
} // namespace Ver_2

namespace Ver_3
{
    template <typename T>
    concept Pointer = std::is_pointer_v<T>;

    template <typename T>
    concept LeanPointer = Pointer<T> && (sizeof(T) == sizeof(void*));

    static_assert(Pointer<int*>);
    static_assert(Pointer<const int*>);
    static_assert(not Pointer<const int>);
    static_assert(not Pointer<int&>);

    template <typename T>
    T max_value(T a, T b)
    {
        return a < b ? b : a;
    }

    template <Pointer T>
    auto max_value(T a, T b)
    {
        assert(a != nullptr);
        assert(b != nullptr);
        return max_value(*a, *b);
    }

    namespace Auto
    {
        auto max_value(Pointer auto a, Pointer auto b)
            requires std::same_as<decltype(a), decltype(b)>
        {
            assert(a != nullptr);
            assert(b != nullptr);
            return max_value(*a, *b);
        }

    } // namespace Auto
} // namespace Ver_3

std::unsigned_integral auto gen_id()
{
    static uint64_t i = 0;
    return ++i;
}

TEST_CASE("concepts")
{
    using namespace Ver_3;

    REQUIRE(max_value(42, 665) == 665);

    int x = 52;
    int y = 665;
    REQUIRE(max_value(&x, &y) == 665);

    std::convertible_to<uint64_t> auto i = gen_id();
}

namespace Ver_4
{
    template <typename T>
    concept ComparableWithNullptr = requires(T obj) {
        obj == nullptr;
        obj != nullptr;
    };

    static_assert(not ComparableWithNullptr<int>);
    static_assert(ComparableWithNullptr<int*>);

    template <typename T>
    concept Pointer = ComparableWithNullptr<T> and requires(T ptr) {
        *ptr;
    };

    template <typename T>
    T max_value(T a, T b)
    {
        return a < b ? b : a;
    }

    template <Pointer T>
    auto max_value(T a, T b)
    {
        assert(a != nullptr);
        assert(b != nullptr);
        return max_value(*a, *b);
    }
} // namespace Ver_4

TEST_CASE("concepts 2")
{
    using namespace Ver_4;

    REQUIRE(max_value(42, 665) == 665);

    int x = 52;
    int y = 665;
    REQUIRE(max_value(&x, &y) == 665);

    auto ptr1 = std::make_shared<int>(53);
    auto ptr2 = std::make_shared<int>(653);

    REQUIRE(max_value(ptr1, ptr2) == 653);
}

template <typename T>
struct Holder
{
    T value;

    Holder(T v) : value(std::move(v))
    {        
    }

    void print() const
    {
        std::cout << "Value: " << value << "\t";
    }

    void print() const requires std::ranges::range<T>
    {
        for(const auto& item : value)
            std::cout << item << " ";
        std::cout << "\n";
    }
};

TEST_CASE("Holder")
{
    std::vector vec = {1, 2, 3};
    Holder h1{vec};
    h1.print();

    Holder h2{std::vector{6, 2, 3}};
    h2.print();

    Holder<int> h3{42};
    h3.print();
}

void add_to_container(auto& container, auto&& value)
{
    if constexpr(requires { container.push_back(value); })  // requires expression
    {
        container.push_back(value);
    }
    else
    {
        container.insert(value);
    }
}

TEST_CASE("requires expression")
{
    std::vector<int> v;
    add_to_container(v, 42);
    REQUIRE(v[0] == 42);
    
    std::set<int> s;
    add_to_container(s, 42);
}

template <typename T>
concept Addable = requires(T a, T b) { 
    {a + b} -> std::same_as<T>;
};

template <typename T>
concept Hashable = requires(T obj) {
    { std::hash<T>{}(obj) } -> std::convertible_to<size_t>;
};

template <Addable T>        
auto add(T a, T b)
{
    return a + b;
}

template <typename T>
concept Sqrtable = requires (T arg) {
    std::sqrt(arg);
};

constexpr size_t default_array_size()
{
    // ...
    return 42;
}

template <typename T>
    requires requires { requires (default_array_size() == 42); } 
void only_42()
{}

TEST_CASE("requires requires")
{
    only_42<int>();
}