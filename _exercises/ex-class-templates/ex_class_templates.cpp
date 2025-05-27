#include <algorithm>
#include <array>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
#include <set>

#include "stack.hpp"

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

// static_assert(std::is_same_v<Stack<int>::container_type, std::deque<int>>);
static_assert(std::is_same_v<TemplateTemplateParam::Stack<int, std::vector>::container_type, std::vector<int>>);

TEST_CASE("After construction", "[stack]")
{
    Stack<int> s;

    SECTION("is empty")
    {
        REQUIRE(s.empty());
    }

    SECTION("size is zero")
    {
        REQUIRE(s.size() == 0);
    }
}

TEST_CASE("Pushing an item", "[stack,push]")
{
    Stack<int> s;

    SECTION("is no longer empty")
    {
        s.push(1);

        REQUIRE(!s.empty());
    }

    SECTION("size is increased")
    {
        auto size_before = s.size();

        s.push(1);

        REQUIRE(s.size() - size_before == 1);
    }

    SECTION("recently pushed item is on a top")
    {
        s.push(4);

        REQUIRE(s.top() == 4);
    }
}

template <typename TStack>
std::vector<typename TStack::value_type> pop_all(TStack& s)
{
    std::vector<typename TStack::value_type> values(s.size());

    for (auto& item : values)
        s.pop(item);

    return values;
}

TEST_CASE("Pop all items")
{
    Stack<int> s;

    s.push(1);
    s.push(4);

    auto values = pop_all<Stack<int>>(s);
    REQUIRE(values.size() == 2);
}

TEST_CASE("Popping an item", "[stack,pop]")
{
    Stack<int> s;

    int x = 1;
    s.push(x);
    s.push(4);

    int item;

    SECTION(" an item from a top to an argument passed by ref")
    {
        s.pop(item);

        REQUIRE(item == 4);
    }

    SECTION("size is decreased")
    {
        size_t size_before = s.size();

        s.pop(item);

        REQUIRE(size_before - s.size() == 1);
    }

    SECTION("LIFO order")
    {
        int a, b;

        s.pop(a);
        s.pop(b);

        REQUIRE(a == 4);
        REQUIRE(b == 1);
    }
}

TEST_CASE("Popping all items")
{
    Stack<int, std::vector<int>> s;

    s.push(1);
    s.push(4);

    auto values = pop_all(s);
    REQUIRE(values.size() == 2);
}


template <typename T>
class IndexedSet : public std::set<T>
{
    using BaseType = std::set<T>;
public:
    using BaseType::BaseType; // inheritance of constructors

    decltype(auto) operator[](size_t index)
    {        
        auto it = std::next(BaseType::begin(), index);
        
        return *it;
    }
};

TEST_CASE("Inherit from std container")
{
    IndexedSet<int> my_set = {1, 2, 4, 5, 6, 7};
    my_set.insert(3);

    REQUIRE(my_set.find(5) != my_set.end());

    REQUIRE(my_set[2] == 3);
}

/////////////////////////////////////////////

struct ShapeBase
{
    virtual void draw() const = 0;
    virtual ~ShapeBase() = default;    
};

struct Circle : public ShapeBase
{
    int radius;

    explicit Circle(int r) : radius{r}
    {}

    ~Circle()
    {
        std::cout << "~Circle()\n";
    }

    void draw() const override
    {
        std::cout << "Drawing circle r=" << radius << "...\n";
    }
};

struct ColorCircle : Circle
{
    using Circle::Circle;

    void draw() const override
    {
        std::cout << "Drawing color circle r=" << radius << "...\n";
    }

    ~ColorCircle()
    {
        std::cout << "~ColorCircle()\n";
    }
};

TEST_CASE("virtual destructors")
{
    // {
    //     std::unique_ptr<ShapeBase> ptr_shape = std::make_unique<ShapeBase>();
    //     ptr_shape->draw();

    //     ptr_shape = std::make_unique<ColorCircle>(100);
    //     ptr_shape->draw();
    // }
    
    std::vector<std::unique_ptr<ShapeBase>> shapes;

    shapes.push_back(std::make_unique<ColorCircle>(100));
    shapes.push_back(std::make_unique<Circle>(160));
    shapes.push_back(std::make_unique<ColorCircle>(100));
    shapes.push_back(std::make_unique<Circle>(160));
    shapes.push_back(std::make_unique<ColorCircle>(100));
    shapes.push_back(std::make_unique<Circle>(160));

    for(const auto& ptr_s : shapes)
        ptr_s->draw();    
}

