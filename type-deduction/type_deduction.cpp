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
}

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

// REFERENCE COLLAPSING
// & &   -> &  // C++98
// && &  -> &
// & &&  -> &
// && && -> &&


void foo(int)
{}

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
}

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

        auto& a1 = x;       // int&
        auto& a2 = cx;      // const int&
        auto& a3 = ref_x;   // int&
        auto& a4 = cref_x;  // const int&
        auto& a5 = tab;     // int (&a5)[10] = tab;
        auto& a6 = foo;     // void (&a6)(int) = foo;
    }

    SECTION("Case 3")
    {
        deduce3(x);  // x - lvalue: deduce3<int&>(int&)            
        deduce3(cx); // cx - lvalue: deduce3<const int&>(const int&)            
        deduce3(ref_x);  // ref_x - lvalue: deduce3<int&>(int&)
        deduce3(cref_x); // cref_x - lvalue: deduce3<const int&>(const int&)         
        deduce3(tab);    // tab - lvalue: deduce3<int(&)[10]>(int(&)[10])            
        deduce3(foo);    // foo - lvalue: deduce3<void(&)(int)>(void(&)(int))        
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

TEST_CASE("decltype(auto)")
{ 
}