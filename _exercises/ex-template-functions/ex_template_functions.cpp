#include <algorithm>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

namespace TODO
{
    namespace Ver_1
    {
        template <typename it, typename func>
        it find_if(it it_begin, it it_end, func check_fn)
        {
            for (it curr_it = it_begin; curr_it != it_end; ++curr_it)
            {
                if (check_fn(*curr_it))
                    return curr_it;
            }
            return it_end;
        }
    } // namespace Ver_1

    inline namespace Ver_2
    {
        auto find_if(auto it_begin, auto sentinel, auto check_fn)
        {
            auto curr_it = it_begin;
            for (; curr_it != sentinel; ++curr_it)
            {
                if (check_fn(*curr_it))
                    return curr_it;
            }

            return curr_it;
        }
    } // namespace Ver_2

} // namespace TODO

struct Unreachable
{
    bool operator==(auto it)
    {
        return false;
    }
};

TEMPLATE_TEST_CASE("my find if", "[vector][list]", (std::vector<int>), (std::list<int>))
{
    SECTION("happy path")
    {
        TestType vec = {1, 2, 3, 4, 665, 124, 4, 55, 234};

        auto is_lesser_evil = [](int x) {
            return x == 665;
        };

        auto pos = TODO::find_if(begin(vec), end(vec), is_lesser_evil);

        REQUIRE(pos != end(vec));
        REQUIRE(*pos == 665);
    }

    SECTION("sad path")
    {
        TestType vec = {1, 2, 3, 4, 124, 4, 55, 234};

        auto is_lesser_evil = [](int x) {
            return x == 665;
        };

        auto pos = TODO::find_if(begin(vec), end(vec), is_lesser_evil);

        REQUIRE(pos == end(vec));
    }

    SECTION("unreachable sentinel")
    {
        TestType vec = {1, 2, 3, 4, 42, 4, 42, 234};

        auto pos = TODO::Ver_2::find_if(vec.begin(), Unreachable{}, [](int x) { return x == 42; });

        REQUIRE(*pos == 42);
    }
}

namespace TODO
{
    template <typename TContainer>
    void zero(TContainer& container)
    {
        //using T = std::remove_cvref_t<decltype(*std::begin(container))>; 
        using T = typename TContainer::value_type;
        T zero_value{};

        for (auto& item : container)
            item = zero_value;
    }
} // namespace TODO

TEST_CASE("zero")
{
    using namespace TODO;

    SECTION("vector<int>")
    {
        std::vector<int> vec = {1, 2, 3, 4};

        zero(vec);

        REQUIRE(vec == std::vector{0, 0, 0, 0});
    }

    SECTION("list<std::string>")
    {
        std::list<std::string> lst = {"one", "two", "three"};

        zero(lst);

        REQUIRE(lst == std::list<std::string>{"", "", ""});
    }
}

namespace TODO
{
    template <typename it, typename sentinel_t>
    auto accumulate(it it_begin, sentinel_t sentinel)
    {
        // using T = typename std::iterator_traits<it>::value_type;
        using T = std::remove_cvref_t<decltype(*it_begin)>; // int

        T result{};

        for (; it_begin != sentinel; ++it_begin)
        {
            result += *it_begin;
        }

        return result;
    }
} // namespace TODO

TEST_CASE("my accumulate")
{
    SECTION("ints")
    {
        const vector<int> data = {1, 2, 3, 4, 5};

        auto result = TODO::accumulate(begin(data), end(data));

        static_assert(std::is_same_v<decltype(result), int>);

        REQUIRE(result == 15);
    }

    SECTION("strings")
    {
        std::string words[] = {"one", "two", "three"};

        auto result = TODO::accumulate(begin(words), end(words));

        static_assert(std::is_same_v<decltype(result), std::string>);

        REQUIRE(result == "onetwothree");
    }
}

struct EndValue42
{
    bool operator==(auto it)
    {
        return *it == 42;
    }
};

TEST_CASE("sentinels")
{
    std::vector<int> vec = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};

    auto result = TODO::accumulate(vec.begin(), EndValue42{});
    REQUIRE(result== 15);
}