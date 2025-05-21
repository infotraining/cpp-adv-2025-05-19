#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <list>
#include <string>
#include <vector>
#include <iostream>

namespace Exercise
{
    // TODO: Implement a function that sets all elements of a container to zero (default value).
    namespace Ver_1
    {
        template <typename T>
        void zero(T& container)
        {
            T newContainer(container.size());
            container = newContainer;
        }
    } // namespace Ver_1

    inline namespace Ver_2
    {
        template <typename Container>
        void zero(Container& container)
        {
            using TValue = typename Container::value_type;

            for (auto& item : container)
            {
                item = TValue{};
            }
        }
    } // namespace Ver_2

} // namespace Exercise

TEST_CASE("zero - Step 1")
{
    using namespace Exercise;

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

void foobar(int)
{}

template <typename T>
void deduce_1(T arg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

template <typename T>
void deduce_2(T& arg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

template <typename T>
void deduce_3(T&& arg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

namespace Cpp20
{
    void deduce_1(auto arg)
    {
        std::cout << __PRETTY_FUNCTION__ << "\n";
    }
}

TEST_CASE("type deduction")
{
    int x = 10;
    const int cx = 42;
    int& ref_x = x;
    const int& cref_x = x;
    int tab[10];   

    SECTION("deduce - auto")
    {
        auto ax1 = x;      // int
        deduce_1(x);         // deduce<int>(int arg)
        Cpp20::deduce_1(x);  // deduce<int>(int arg)

        auto ax2 = cx;     // int - const is removed
        deduce_1(cx);      // deduce<int>(int)

        auto ax3 = ref_x;  // int - ref is removed
        deduce_1(ref_x);   // deduce<int>(int)

        auto ax4 = cref_x; // int - const ref is removed
        deduce_1(cref_x);  // deduce<int>(int)

        auto ax5 = tab;    // int* - array decays into pointer
        deduce_1(tab);     // deduce<int*>(int*)

        auto ax6 = foobar; // void(*ax6)(int)
        deduce_1(foobar);  // deduce<void(*)(int)>(void(*)(int))
    }

    SECTION("deduce - auto&")
    {
        auto& ax1 = x;      // int&
        deduce_2(x);        // deduce_2<int>(int&)

        auto& ax2 = cx;     // const int& - const is not removed
        deduce_2(cx);       // deduce_2<const int>(const int&)

        auto& ax3 = ref_x;  // int&
        deduce_2(ref_x);    // deduce_2<int>(int&)

        auto& ax4 = cref_x; // const int&
        deduce_2(cref_x);   // deduce_2<const int>(const int&)

        auto& ax5 = tab;    // int(&ax5)[10]
        deduce_2(tab);      // deduce_2<int[10]>(int(&)[10])

        auto& ax6 = foobar; // void(&ax6)(int)
        deduce_2(foobar);   // deduce_2(void(int))(void(&)(int))
    }

    SECTION("deduce - auto&&")
    {
        SECTION("lvalue")
        {
            auto&& ax1 = x;  // int&
            deduce_3(x);     // deduce_3<int&>(int&)
        }

        SECTION("rvalue")
        {
            auto&& ax2 = 42; // int&&
            deduce_3(42);    // deduce_3<int>(int&&)
        }
    }
}