#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

// NTTP
template <typename T, size_t N>
struct Array
{
    T items[N];

    using iterator = T*;
    using const_iterator = const T*;
    using reference = T&;
    using const_reference = const T&;

    constexpr size_t size() const
    {
        return N;
    }

    constexpr iterator begin() 
    {
        return items;
    }

    constexpr iterator end()
    {
        return items + N;
    }

    constexpr const_iterator begin() const
    {
        return items;
    }

    constexpr const_iterator end() const
    {
        return items + N;
    }

    constexpr reference operator[](size_t index)
    {
        return items[index];
    }

    constexpr const_reference operator[](size_t index) const
    {
        return items[index];
    }
};

TEST_CASE("class templates")
{
    Array<int, 10> arr1 = {};

    static_assert(arr1.size() == 10);

    for(auto& item : arr1)
        item = 0;
}

template <double Factor, typename T>
auto scale(T x)
{
    return x * Factor;
}

struct Coefficients
{
    int a;
    int b;
};

template <Coefficients C, typename T>
auto scale_by(T x)
{
    return C.a * x + C.b;
}

template <std::pair<int, int> C, typename T>
auto scale_with_pair(T x)
{
    return C.first * x + C.second;
}

template <auto Function, typename T>
auto scale_by_func(T x)
{
    return Function(x);
}

TEST_CASE("C++20")
{
    REQUIRE(scale<2.0>(8) == 16.0);
    REQUIRE(scale_by<Coefficients{1, 2}>(3) == 5);
    REQUIRE(scale_with_pair<std::pair{1, 10}>(2) == 12);
    REQUIRE(scale_by_func<[](int x) { return 42 * x; }>(2) == 84);
}