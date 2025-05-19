#include "helpers.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace std::literals;

std::string full_name(const std::string& first_name, const std::string& last_name)
{
    return first_name + " " + last_name;
}

TEST_CASE("reference binding")
{
    std::string name = "jan";

    SECTION("C++98")
    {
        std::string& ref_name = name; // LVALUE is bounded with LValueRef

        const std::string& ref_full_name = full_name(name, "Kowalski"); // RVALUE can be bound to LValueRef to CONST

        std::cout << ref_full_name << "\n";
        //ref_full_name[0] = 'J'; 
    }

    SECTION("C++11")
    {
        std::string&& ref_full_name = full_name(name, "Kowalski"); // RVALUE can be bound to RValueRef
        std::cout << ref_full_name << "\n";
        ref_full_name[0] = 'J';
        std::cout << ref_full_name << "\n";

        //std::string&& ref_name = name; // LVALUE cannot be bound to RValueRef
    }
}