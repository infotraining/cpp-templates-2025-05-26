#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

// template variable pi_v
template <typename T>
constexpr T pi_v = 3.1415;

TEST_CASE("pi")
{
    REQUIRE(pi_v<double> == 3.1415);
}

constexpr int by_2(int a)
{
    return a * 2;
}

template <size_t a>
struct By2
{
    static constexpr int value = 2 * a;
};

// template variable
template <size_t a>
constexpr int By2_v = By2<a>::value;

TEST_CASE("type traits")
{
    CHECK(by_2(2) == 4);
    static_assert(by_2(8) == 16);

    static_assert(By2<2>::value == 4);
    static_assert(By2_v<2> == 4);    
}

template <typename T>
struct Identity
{
    using type = T;
};

template <typename T>
using Identity_t = typename Identity<T>::type;

//////////////////////////////////////////////

template <typename T1, typename T2>
struct IsSame
{
    static constexpr bool value = false;
};

template <typename T>
struct IsSame<T, T>
{
    static constexpr bool value = true;
};

template <typename T1, typename T2>
constexpr bool IsSame_v = IsSame<T1, T2>::value;

TEST_CASE("identity")
{
    using T = int;

    Identity<int>::type x;
    Identity_t<double> dx;
    
    static_assert(IsSame<int, double>::value == false);
    static_assert(IsSame<T, int>::value);
    static_assert(IsSame_v<Identity<int>::type, int>);    
}

///////////////////////////////////////
// Factorial

template <size_t N>
struct Factorial 
{
    const static size_t value = Factorial<N-1>::value * N;
};

template <>
struct Factorial<1>
{
    const static size_t value = 1;
};

constexpr size_t factorial(size_t n)
{
    return (n == 1) ? 1 : factorial(n - 1) * n;
}

static_assert(Factorial<14>::value);

///////////////////////////////////////////////////
// TODO

TEST_CASE("IsPointer")
{
    static_assert(IsPointer_v<int*> == true);
    static_assert(IsPointer_v<int> == false);
}

TEST_CASE("IsVoid")
{
    static_assert(IsVoid_v<void> == true);
    static_assert(IsVoid_v<int> == false);
}