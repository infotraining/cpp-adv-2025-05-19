#include "helpers.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using Helpers::Gadget;

template <typename T>
T maximum(T a, T b)
{
    return a < b ? b : a;
}

template <>
const char* maximum(const char* a, const char* b)
{
    return std::strcmp(a, b) < 0 ? b : a;
}

TEST_CASE("function templates")
{
    REQUIRE(maximum<int>(5, 42) == 42);
    REQUIRE(maximum(5.5, 42.6) == 42.6);
    REQUIRE(maximum(std::string{"abc"}, std::string{"def"}) == "def");

    REQUIRE(maximum<double>(5, 42.7) == 42.7);
    REQUIRE(maximum(static_cast<double>(5), 42.7) == 42.7);

    const char* c_txt_1 = "abcz";
    const char* c_txt_2 = "abcd";
    REQUIRE(maximum(c_txt_1, c_txt_2) == std::string("abcz"));

    auto ptr_fun = &maximum<int>; // address of template function
}

namespace ReturnTypeFromTemplateFunction
{
    namespace Ver_1
    {
        template <typename TResult, typename T1, typename T2>
        TResult maximum(T1 a, T2 b)
        {
            return a < b ? a : b;
        }
    } // namespace Ver_1

    namespace Ver_2
    {
        template <typename T1, typename T2>
        auto maximum(T1 a, T2 b)
        {
            return a < b ? a : b;
        }
    }

    namespace Ver_3
    {
        template <typename T1, typename T2>
        std::common_type_t<T1, T2> maximum(T1 a, T2 b)
        {
            if (a < b)
                return b;
            return a;
        }
    }
} // namespace ReturnTypeFromTemplateFunction

TEST_CASE("return types from template functions")
{
    int a = 42;
    double b = 5.5;

    REQUIRE(ReturnTypeFromTemplateFunction::Ver_1::maximum<double>(a, b) == 42.0);
    REQUIRE(ReturnTypeFromTemplateFunction::Ver_2::maximum(a, b) == 42.0);
    REQUIRE(ReturnTypeFromTemplateFunction::Ver_3::maximum(a, b) == 42.0);
}

TEST_CASE("class templates")
{
    // TODO
}

TEST_CASE("template aliases")
{
    // TODO
}

TEST_CASE("template variables")
{
    // TODO
}