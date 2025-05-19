#define ENABLE_MOVE_SEMANTICS
#include "helpers.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <iostream>

using namespace std::literals;

namespace Explain
{
    template <typename T>
    class vector
    {
        std::vector<T> item_;
    public:
        void push_back(const T& item)
        {
            std::cout << "copy of " << item << " to vector\n";
        }

        void push_back(T&& item)
        {
            std::cout << "move of " << item << " to vector\n";
        }

        template <typename TItem>
        void push_back_alt(TItem&& item)
        {
            item_.push_back(std::forward<TItem>(item));
        }
    };
}

Helpers::Vector create_and_fill()
{
    using Helpers::Vector, Helpers::String;

    Vector vec;

    String str = "very, very, very, very, very, very, very, very, very, very, very, very, very, very, very, very long text";

    vec.push_back(str);

    vec.push_back(str + str); 

    vec.push_back("text"); 

    vec.push_back(std::move(str));  

    return vec;
}

TEST_CASE("move semantics motivation")
{
    Helpers::Vector vec = create_and_fill();

    Helpers::String::print_stats("Total");

    Explain::vector<Helpers::String> my_vec;

    my_vec.push_back(vec[0]);
    my_vec.push_back(Helpers::String("text"));
}

struct MyValue
{
    int id;
    std::string value;

    // MyValue(int id, const std::string& v) : id(id), value(v)
    // {}

    // MyValue(int id, std::string&& v) : id(id), value(std::move(v))
    // {}

    MyValue(int id, std::string v) : id(id), value(std::move(v))
    {}
};

TEST_CASE("moving primitive types")
{
    int value = 42;
    int target = std::move(value);

    REQUIRE(value == 42);
    REQUIRE(target == 42);
}

TEST_CASE("default special functions")
{
    std::string name = "mv1";
    MyValue mv1{1, name};

    MyValue mv2{2, "mv2"};

    MyValue mv3 = std::move(mv1); // mv

    REQUIRE(mv1.id == 1);
    REQUIRE(mv1.value == "");

    REQUIRE(mv3.id == 1);
    REQUIRE(mv3.value == "mv1");
}


struct X
{
    std::string name;

    X() = default;

    X(std::string n) : name(std::move(n))
    {}
    
    // X(const X&) = default;
    // X& operator=(const X&) = default;
    // X(X&&) = default;
    // X& operator=(X&&) = default;

    // ~X()
    // {
    //     std::cout << "~X(" << name << ")\n";
    // }

    friend std::ostream& operator<<(std::ostream& out, const X& obj)
    {
        return out << "X(" << obj.name << ")";
    }
};

TEST_CASE("special functions")
{
    X x("Text");
    std::cout << "x = " << x << "\n";

    X copy_x = x;
    std::cout << "copy_x = " << copy_x << "\n";

    X target = std::move(x);
    std::cout << "target = " << target << "\n";

    std::cout << "x after move = " << x << "\n";
}