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
    public:
        void push_back(const T& item)
        {
            std::cout << "copy of " << item << " to vector\n";
        }

        void push_back(T&& item)
        {
            std::cout << "move of " << item << " to vector\n";
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

    MyValue(int id, const std::string& v) : id(id), value(v)
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
    MyValue mv1{1, "mv1"};

    MyValue mv2 = mv1; // cc
    MyValue mv3 = std::move(mv1); // mv

    REQUIRE(mv1.id == 1);
    REQUIRE(mv1.value == "");

    REQUIRE(mv3.id == 1);
    REQUIRE(mv3.value == "mv1");
}