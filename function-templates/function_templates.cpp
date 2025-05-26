#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

template <typename T>
T max_value(T a, T b)
{
    return a < b ? b : a;
}

// template <>
// const char* max_value(const char* a, const char* b)
// {
//     return std::strcmp(a, b) < 0 ? b : a;
// }

const char* max_value(const char* a, const char* b)
{
    return std::strcmp(a, b) < 0 ? b : a;
}

namespace Ver_2
{
    template <typename T1, typename T2>
    auto max_value(T1 a, T2 b)
    {
        return a < b ? b : a;
    }
}

template <typename T>
void foo(T arg)
{
    unknown(arg);
}

TEST_CASE("function templates")
{
    SECTION("int")
    {
        int a = 19;
        int b = 42;

        int result = max_value(a, b);
        REQUIRE(result == 42);
        REQUIRE(max_value(a, 665) == 665);
    }

    SECTION("double")
    {
        double dx = 0.01;
        double dy = 0.1;

        double result = max_value(dx, dy);
        REQUIRE(result == 0.1);
    }

    SECTION("std::string")
    {
        std::string str1 = "ala";
        std::string str2 = "ola";

        REQUIRE(max_value(str1, str2) == "ola");

        const std::string& result = max_value(str2, std::string("zola"));
        REQUIRE(result == "zola");
    }

    SECTION("const char*")
    {
        const char* txt1 = "ola";
        const char* txt2 = "ala";

        REQUIRE(max_value(txt1, txt2) == "ola"s);
    }

    SECTION("resolving conflicts during deduction")
    {
        int a = 42;
        double pi = 3.14;

        SECTION("static_cast")
        {
            REQUIRE(max_value(a, static_cast<int>(pi)) == 42);
            REQUIRE(max_value(static_cast<double>(a), pi) == 42.0);
        }

        SECTION("turn-off deduction")
        {
            REQUIRE(max_value<double>(a, pi) == 42.0);
        }
    }
}

TEST_CASE("partial deduction for function templates")
{
    REQUIRE(Ver_2::max_value<double>(3.14f, 42) == 42.0);
}

struct Person
{
    std::string name;
    uint8_t age;

    auto operator<=>(const Person& other) const = default;
};

TEST_CASE("custom types")
{
    Person p1{"Jan", 33};
    Person p2{"Jan", 42};

    Person result = max_value(p1, p2);
    REQUIRE(max_value(p1, p2) == p2);
}

TEST_CASE("address of template function")
{
    auto ptr_fun = &max_value<int>;
}

////////////////////////////////////////////////////////
// Result type in template functions

namespace Ver_1
{
    template <typename TResult, typename T1, typename T2>
    TResult add(T1 a, T2 b)
    {
        return a + b;
    }
} // namespace Ver_1

namespace Ver_2
{
    template <typename T1, typename T2>
    auto add(T1 a, T2 b)
    {
        return a + b;
    }

    template <typename T1, typename T2>
    auto maximum_value(T1 a, T2 b)
    {
        if (a < b)
            return b;
        return a;
    }
} // namespace Ver_2

namespace Ver_3
{
    template <typename T1, typename T2>
    std::common_type_t<T1, T2> add(T1 a, T2 b)
    {
        return a + b;
    }
}

namespace Ver_4
{
    template <typename T1, typename T2, typename TResult = std::common_type_t<T1, T2>>
    TResult add(T1 a, T2 b)
    {
        return TResult(a) + b;
    }
}

TEST_CASE("result type in template function")
{
    SECTION("explicit template param")
    {
        using namespace Ver_1;
        auto result1 = add<double>(4, 3.14);
        auto result2 = add<double>(3.14, 4);

        auto ptr1 = std::make_unique<int>(42);
        auto ptr2 = std::make_unique<double>(42.9);
    }

    SECTION("auto")
    {
        using namespace Ver_2;

        auto result1 = add(3.14, 4);
        //auto result2 = Ver_2::maximum_value(3.14, 4);
    }

    SECTION("type-traits")
    {
        auto result1 = Ver_3::add(1, 3.14);
        auto result2 = Ver_3::add("text"s, ":abc");        
        REQUIRE(result2 == "text:abc");
    }

    SECTION("default param")
    {
        auto result1 = Ver_4::add(4, 3.14);
        auto result2 = Ver_4::add<const char*, const char*, std::string>("text", ":abc");
    }
}