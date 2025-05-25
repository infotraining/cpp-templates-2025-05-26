#include <algorithm>
#include <catch2/catch_approx.hpp>
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
    // TODO: find_if
} // namespace TODO

TEST_CASE("my find if")
{
//     SECTION("happy path")
//     {
//         vector<int> vec = {1, 2, 3, 4, 665, 124, 4, 55, 234};

//         auto is_lesser_evil = [](int x) {
//             return x == 665;
//         };

//         auto pos = TODO::find_if(begin(vec), end(vec), is_lesser_evil);

//         REQUIRE(pos != end(vec));
//         REQUIRE(*pos == 665);
//     }

//     SECTION("sad path")
//     {
//         vector<int> vec = {1, 2, 3, 4, 124, 4, 55, 234};

//         auto is_lesser_evil = [](int x) {
//             return x == 665;
//         };

//         auto pos = TODO::find_if(begin(vec), end(vec), is_lesser_evil);

//         REQUIRE(pos == end(vec));
//     }
}

namespace TODO
{
    // TODO: accumulate
} // namespace TODO

TEST_CASE("my accumulate")
{
    // SECTION("ints")
    // {
    //     const vector<int> data = {1, 2, 3, 4, 5};

    //     auto result = TODO::accumulate(begin(data), end(data));

    //     static_assert(std::is_same_v<decltype(result), int>);

    //     REQUIRE(result == 15);
    // }

    // SECTION("strings")
    // {
    //     std::string words[] = {"one", "two", "three"};

    //     auto result = TODO::accumulate(begin(words), end(words));

    //     static_assert(std::is_same_v<decltype(result), std::string>);

    //     REQUIRE(result == "Text: onetwothree");
    // }
}
