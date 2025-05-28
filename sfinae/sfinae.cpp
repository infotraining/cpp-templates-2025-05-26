#include <array>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

namespace Ver_1
{
    template <typename T>
    bool is_power_of_2(T value)
    {
        using TValue = std::remove_pointer_t<T>;
        TValue current_value;

        if constexpr (std::is_pointer_v<T>)
        {
            assert(value != nullptr);
            current_value = *value;
        }
        else
        {
            current_value = value;
        }

        if constexpr (std::is_integral_v<TValue>)
        {
            return current_value > 0 && (current_value & (current_value - 1)) == 0;
        }
        else
        {
            int exponent;
            const TValue mantissa = std::frexp(current_value, &exponent);
            return mantissa == static_cast<TValue>(0.5);
        }
    }
} // namespace Ver_1

inline namespace Ver_2
{
    template <typename T>
    concept Pointer = std::is_pointer_v<T>;

    template <std::integral T>
    bool is_power_of_2(T value)
    {
        return value > 0 && (value & (value - 1)) == 0;
    }

    template <typename T>
        requires std::floating_point<T>
    bool is_power_of_2(T value)
    {
        int exponent;
        const T mantissa = std::frexp(value, &exponent);
        return mantissa == static_cast<T>(0.5);
    }

    template <Pointer T>
    bool is_power_of_2(T value)
    {
        return is_power_of_2(*value);
    }
} // namespace Ver_2

// template <typename T>
// bool is_power_of_2(T value)
// {
//     int exponent;
//     const T mantissa = std::frexp(value, &exponent);
//     return mantissa == static_cast<T>(0.5);
// }

TEST_CASE("is_power_of_2")
{
    REQUIRE(is_power_of_2(8L));
    REQUIRE(is_power_of_2(32ULL));
    REQUIRE(is_power_of_2(1024));
    REQUIRE(is_power_of_2(1025) == false);
    REQUIRE(is_power_of_2(665) == false);

    REQUIRE(is_power_of_2(8.0));
    REQUIRE(is_power_of_2(1024.0));
    REQUIRE(is_power_of_2(1024.0f));

    int x = 8;
    REQUIRE(is_power_of_2(&x));

    double dx = 512.0;
    REQUIRE(is_power_of_2(&dx));
}

template <size_t N = 256>
auto create_buffer()
{
    if constexpr (N < 512)
    {
        return std::array<int, N>{};
    }
    else
    {
        return std::vector<int>(N);
    }
}

namespace LegacyCode
{
    template <typename T>
    void resize(T& container)
    { }

    template <typename T>
    void resize(std::vector<T>& vec, size_t size)
    {
        vec.resize(size);
    }

    template <size_t N = 256>
        std::conditional < N<512, std::array<int, N>, std::vector<int>> create_buffer()
    {
        using TContainer = std::conditional < N<512, std::array<int, N>, std::vector<int>>;

        TContainer container{};

        resize(container, N);

        return container;
    }
} // namespace LegacyCode

TEST_CASE("auto + if_constexpr")
{
    auto buffer = create_buffer<1024>();

    for (auto& item : buffer)
    {
        item = 665;
    }
}

inline namespace Explain
{
    template <bool Condition, typename T = void>
    struct EnableIf
    {
        using type = T;
    };

    template <typename T>
    struct EnableIf<false, T>
    {
    };

    template <bool Condition, typename T = void>
    using EnableIf_t = typename EnableIf<Condition, T>::type;
} // namespace Explain

template <typename T>
EnableIf_t<std::is_integral_v<T>> foobar(T x)
{
    std::cout << "foobar(integral x)\n";
}

template <typename T>
EnableIf_t<std::is_floating_point_v<T>> foobar(T x)
{
    std::cout << "foobar(floating x)\n";
}

TEST_CASE("SFINAE")
{
    foobar(42L);

    foobar(5.0);
}