#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <list>
#include <string>
#include <vector>

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

    template <typename Container>
    void zero(Container& container)
    {
        using TValue = typename Container::value_type;

        for (auto& item : container)
        {
            item = TValue{};
        }
    }

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