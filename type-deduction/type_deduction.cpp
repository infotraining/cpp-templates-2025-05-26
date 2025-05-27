#include "catch.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

template <typename T>
void deduce1(T arg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

namespace Cpp20
{
    void deduce1(auto arg)
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
    }
} // namespace Cpp20

template <typename T>
void deduce2(T& arg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

template <typename T>
void deduce3(T&& arg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

namespace Cpp20
{
    void deduce3(auto&& arg)
    {
    }
} // namespace Cpp20

struct Container
{
    std::vector<std::string> items;

    // void add(const std::string& item)
    // {
    //     items.push_back(item); // insert copy of item
    // }

    // void add(std::string&& item)
    // {
    //     items.push_back(std::move(item));
    // }

    // perfect forwarding
    template <typename TItem>
    void add(TItem&& item) // universal reference
    {
        items.push_back(std::forward<TItem>(item));
    }
};

namespace Explain
{
    template <typename T>
    decltype(auto) forward(T&& arg)
    {
        if constexpr (std::is_lvalue_reference_v<T>)
        {
            return arg;
        }
        else
        {
            return std::move(arg);
        }
    }
} // namespace Explain

TEST_CASE("perfect forwarding")
{
    Container container;

    std::string str = "text";
    container.add(str); // lvalue

    container.add(std::string("abc"));
}

// REFERENCE COLLAPSING
// & &   -> &  // C++98
// && &  -> &
// & &&  -> &
// && && -> &&

void foo(int)
{ }

namespace Explain
{
    template <typename TContainer>
    size_t size(const TContainer& container)
    {
        return container.size();
    }

    template <typename T, size_t N>
    constexpr size_t size(T (&arr)[N])
    {
        return N;
    }
} // namespace Explain

TEST_CASE("size")
{
    std::vector<int> vec = {1, 2, 3};
    REQUIRE(Explain::size(vec) == 3);

    int native_tab[10] = {1, 2, 3};
    static_assert(Explain::size(native_tab) == 10);
}

TEST_CASE("type deduction rules")
{
    int x = 10;
    const int cx = 10;
    int& ref_x = x;
    const int& cref_x = x;
    int tab[10];
    static_assert(sizeof(tab) == 10 * sizeof(int));

    SECTION("Case 1")
    {
        deduce1(x);      // deduce1<int>(int)
        deduce1(cx);     // deduce1<int>(int)
        deduce1(ref_x);  // deduce1<int>(int)
        deduce1(cref_x); // deduce1<int>(int)
        deduce1(tab);    // deduce1<int*>(int*)  - decays to pointer
        deduce1("text"); // deduce1<const char*>(const char*)
        deduce1(foo);    // deduce1<void(*)(int)>(void(*)(int))

        auto a1 = x;      // int
        auto a2 = cx;     // int
        auto a3 = ref_x;  // int
        auto a4 = cref_x; // int
        auto a5 = tab;    // int*  - array decays to pointer
        auto a6 = foo;    // void(*a6)(int) - function decays to pointer
    }

    SECTION("Case 2")
    {
        deduce2(x);      // deduce2<int>(int&)
        deduce2(cx);     // deduce2<const int>(const int&)
        deduce2(ref_x);  // deduce2<int>(int&)
        deduce2(cref_x); // deduce2<const int>(const int&)
        deduce2(tab);    // deduce2<int[10]>(int(&)[10])
        deduce2(foo);    // deduce2<void(int)>(void(&)(int))

        auto& a1 = x;      // int&
        auto& a2 = cx;     // const int&
        auto& a3 = ref_x;  // int&
        auto& a4 = cref_x; // const int&
        auto& a5 = tab;    // int (&a5)[10] = tab;
        auto& a6 = foo;    // void (&a6)(int) = foo;
    }

    SECTION("Case 3")
    {
        deduce3(x);              // x - lvalue: deduce3<int&>(int&)
        deduce3(cx);             // cx - lvalue: deduce3<const int&>(const int&)
        deduce3(ref_x);          // ref_x - lvalue: deduce3<int&>(int&)
        deduce3(cref_x);         // cref_x - lvalue: deduce3<const int&>(const int&)
        deduce3(tab);            // tab - lvalue: deduce3<int(&)[10]>(int(&)[10])
        deduce3(foo);            // foo - lvalue: deduce3<void(&)(int)>(void(&)(int))
        deduce3(string("text")); // string("text") - rvalue: deduce3<std::string>(std::string&&)

        auto&& a1 = x;
        auto&& a2 = cx;
        auto&& a3 = ref_x;
        auto&& a4 = cref_x;
        auto&& a5 = tab;
        auto&& a6 = foo;
        auto&& a7 = string("text");
    }
}

template <typename T>
auto multiply(T a, T b)
{
    return a * b;
}

TEST_CASE("decltype(auto)")
{
    int x = 42;
    const std::unordered_map<std::string, int> dict = { {"one", 1}, {"two", 2} };

    auto ax1 = x;
    auto ax2 = dict;

    REQUIRE(ax2.size() == 2);

    decltype(x) d1{};
    decltype(dict) d2;
}

template <typename TContainer>
decltype(auto) get_nth(TContainer& container, size_t nth)
{
    std::remove_reference_t<decltype(container)> other_container;

    return container[nth]; 
}

constexpr int foobar()
{
    int* ptr = new int[10];

    ptr[1] = 2;

    delete[] ptr;
    return 53;
}

TEST_CASE("foobar")
{
    constexpr int x = foobar();
}

TEST_CASE("auto vs. decltype(auto)")
{
    std::vector<int> vec = {1, 2, 3};

    auto result = get_nth(vec, 1);
    REQUIRE(result == 2);

    get_nth(vec, 1) = 665;
    REQUIRE(vec[1] == 665);

    std::vector<std::string> words = { "one", "two" };

    get_nth(words, 0) = "new";
    REQUIRE(words[0] == "new");

    std::vector<bool> vec_bool = {0, 1, 1, 1};

    get_nth(vec_bool, 1) = 0;
    REQUIRE(vec_bool[1] == 0);
}