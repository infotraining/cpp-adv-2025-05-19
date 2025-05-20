
#include "paragraph.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <memory>

using namespace std;

TEST_CASE("Moving paragraph")
{
    LegacyCode::Paragraph txt("***");
    LegacyCode::Paragraph target_txt = std::move(txt);

    REQUIRE(target_txt.get_paragraph() == string("***"));
    REQUIRE(txt.get_paragraph() == nullptr);
}

TEST_CASE("Moving text shape")
{
    Text txt{10, 20, "text"};
    Text target_txt = std::move(txt);

    REQUIRE(target_txt.text() == string("text"));
    REQUIRE(txt.text() == string());
}

TEST_CASE("ShapeGroup")
{
    ShapeGroup sg;
    sg.add(std::make_unique<Text>(10, 20, "text")); // uncomment this line

    REQUIRE(sg.shapes.size() == 1);

    Text& t = dynamic_cast<Text&>(*sg.shapes[0]);
    REQUIRE(t.text() == "text"s);
}

namespace Exercise
{
    namespace Step1
    {
        std::string exchange(std::string& ov, const std::string& nv /* lvalue */)
        {
            std::string result = std::move(ov); // move semantics - move constructor
            ov = nv;                            // copy assignment

            return result;
        }

        std::string exchange(std::string& ov, std::string&& nv /* rvalue */)
        {
            std::string result = std::move(ov); // move semantics - move constructor
            ov = std::move(nv);                 // move assignment

            return result;
        }
    } // namespace Step1

    namespace Step2
    {
        template <typename TNewValue>
        std::string exchange(std::string& ov, TNewValue&& nv)
        {
            std::string result = std::move(ov);
            ov = std::forward<TNewValue>(nv);

            return result;
        }
    } // namespace Step2

    inline namespace Step3
    {
        template <typename TOldValue, typename TNewValue>
        TOldValue exchange(TOldValue& ov, TNewValue&& nv)
        {
            TOldValue result = std::move(ov);
            ov = std::forward<TNewValue>(nv);
            return result;
        }

    } // namespace Step3

} // namespace Exercise

TEST_CASE("exchange")
{
    SECTION("lvalues")
    {
        std::string v1 = "old value";
        std::string v2 = "new value";

        std::string result = Exercise::exchange(v1, v2); //   result <- v1 <- v2

        REQUIRE(v1 == "new value");
        REQUIRE(result == "old value");
    }

    SECTION("rvalues")
    {
        std::string v1 = "old value";

        std::string result = Exercise::exchange(v1, std::string{"new value"}); //   result <- v1 <- rvalue

        REQUIRE(v1 == "new value");
        REQUIRE(result == "old value");
    }
}