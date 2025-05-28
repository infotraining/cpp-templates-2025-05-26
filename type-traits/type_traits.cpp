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

///////////////////////////////////////////////
// Identity type trait

template <typename T>
struct Identity
{
    using type = T;
};

template <typename T>
using Identity_t = typename Identity<T>::type;

//////////////////////////////////////////////
// Integral constant

template <typename T, T value> 
struct IntegralConstant
{
    static constexpr T value = value;
};

template <typename T, T value>
constexpr T IntegralConstant_v = IntegralConstant<T, value>::value;

static_assert(IntegralConstant<int, 42>::value == 42);

/////////////////////////////////////////////
// Boolean constant

template <bool B>
using BoolConstant = IntegralConstant<bool, B>;

static_assert(BoolConstant<true>::value == true);
static_assert(BoolConstant<false>::value == false);

////////////////////////////////////////////////////
// TrueType and FalseType

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;

////////////////////////////////////////////////
// IsSame type trait

template <typename T1, typename T2>
struct IsSame : FalseType
{
};

template <typename T>
struct IsSame<T, T> : TrueType
{
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
// IsPointer

template <typename T>
struct IsPointer
{
    static constexpr bool value = false;
};

template <typename T>
struct IsPointer<T*>
{
    static constexpr bool value = true;
};

template <typename T>
constexpr bool IsPointer_v = IsPointer<T>::value;

//////////////////////////////////////////////
// IsVoid

template <typename T>
struct IsVoid
{
    static constexpr bool value = false;
};

template <>
struct IsVoid<void>
{
    static constexpr bool value = true;
};

template <typename T>
constexpr bool IsVoid_v = IsVoid<T>::value;

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

//////////////////////////////////////////////
// Remove reference

template <typename T>
struct RemoveReference
{
    using type = T;
};

template <typename T>
struct RemoveReference<T&>
{
    using type = T;
};

template <typename T>
struct RemoveReference<T&&>
{
    using type = T;
};

template <typename T>
using RemoveReference_t = typename RemoveReference<T>::type;

////////////////////////////////////////////////
// Zeroing out a container

template <typename TRange>
void zero(TRange& container)
{
    using T = RemoveReference_t<decltype(*std::begin(container))>;
    T zero{};

    for(auto& item : container)
        item = zero;
}

TEST_CASE("remove reference")
{
    RemoveReference_t<int&> x1;

    std::vector<int> vec = {1, 2, 3};
    zero(vec);
}