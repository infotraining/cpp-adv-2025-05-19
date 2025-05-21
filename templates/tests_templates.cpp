#include "helpers.hpp"

#include <catch2/catch_test_macros.hpp>
#include <deque>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>

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
    } // namespace Ver_2

    namespace Ver_3
    {
        template <typename T1, typename T2>
        std::common_type_t<T1, T2> maximum(T1 a, T2 b)
        {
            if (a < b)
                return b;
            return a;
        }
    } // namespace Ver_3

    namespace Ver_4
    {
        template <typename T1, typename T2, typename TResult = std::common_type_t<T1, T2>>
        TResult maximum(T1 a, T2 b)
        {
            return a < b ? a : b;
        }
    } // namespace Ver_4
} // namespace ReturnTypeFromTemplateFunction

TEST_CASE("return types from template functions")
{
    int a = 42;
    double b = 5.5;

    REQUIRE(ReturnTypeFromTemplateFunction::Ver_1::maximum<double>(a, b) == 42.0);
    REQUIRE(ReturnTypeFromTemplateFunction::Ver_2::maximum(a, b) == 42.0);
    REQUIRE(ReturnTypeFromTemplateFunction::Ver_3::maximum(a, b) == 42.0);
    REQUIRE(ReturnTypeFromTemplateFunction::Ver_4::maximum(a, b) == 42);
}

template <typename T, typename TContainer = std::vector<T>>
class Stack
{
    TContainer items_;

public:
    using value_type = T;
    using reference_type = T&;
    using const_reference_type = const T&;

    Stack() = default;

    bool empty() const
    {
        return items_.empty();
    }

    // void push(const T& item)
    // {
    //     items_.push_back(item);
    // }

    // void push(T&& item)
    // {
    //     items_.push_back(std::move(item));
    // }

    template <typename TItem>
    void push(TItem&& item)
    {
        items_.push_back(std::forward<TItem>(item));
    }

    size_t size() const;

    const_reference_type top() const
    {
        return items_.back();
    }

    void pop()
    {
        items_.pop_back();
    }
};

template <typename T, typename TContainer>
size_t Stack<T, TContainer>::size() const
{
    return items_.size();
}

TEST_CASE("class templates")
{
    Stack<int, std::deque<int>> stack_int;

    stack_int.push(42);
    stack_int.push(665);
    stack_int.push(15);

    REQUIRE(stack_int.top() == 15);
    stack_int.pop();
    REQUIRE(stack_int.size() == 2);
}

template <typename Container>
void zero(Container& container)
{
    using TValue = typename Container::value_type;

    for (auto& item : container)
    {
        item = TValue{};
    }
}

template <typename T, size_t N>
struct Array
{
    using iterator = T*;
    using const_iterator = const T*;
    using value_type = T;
    using reference = T;
    using const_reference = T&;

    T items[N];

    size_t size() const
    {
        return N;
    }

    reference operator[](size_t index)
    {
        return items[index];
    }

    const_reference operator[](size_t index) const
    {
        return items[index];
    }

    iterator begin()
    {
        return items;
    }

    const_iterator begin() const
    {
        return items;
    }

    iterator end()
    {
        return items + N;
    }

    const_iterator end() const
    {
        return items + N;
    }
};

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& out, const Array<T, N>& arr)
{
    out << "[ ";
    for (const auto& item : arr)
        out << item << " ";
    out << "]";
    return out;
}

template <typename T>
void print(const T& container)
{
    std::cout << "[ " << std::size(container) << "; ";
    for (const auto& item : container)
    {
        std::cout << item << " ";
    }
    std::cout << "]\n";
}

TEST_CASE("NTTP - Non-Type Template Params")
{
    Array<int, 32> buffer;

    zero(buffer);
    print(buffer);

    std::cout << "buffer: " << buffer << "\n";
}

namespace Explain
{
    template <typename TContainer>
    size_t size(const TContainer& container)
    {
        return container.size();
    }

    template <typename T, size_t N>
    size_t size(const T (&arr)[N])
    {
        return N;
    }
} // namespace Explain

TEST_CASE("deducing size of array")
{
    std::vector<int> vec = {1, 2, 3};
    print(vec);
    REQUIRE(Explain::size(vec) == 3);

    int tab[5] = {1, 2, 3, 4, 5};
    REQUIRE(Explain::size(tab) == 5);
    print(tab);
}
namespace TemplateTemplateParams
{
    template <typename T, 
              template <typename, typename> class Container = std::vector, 
              typename TAllocator = std::allocator<T>>
    class Stack
    {
        Container<T, TAllocator> items_;

    public:
        using value_type = T;
        using reference_type = T&;
        using const_reference_type = const T&;

        Stack() = default;

        bool empty() const
        {
            return items_.empty();
        }

        void push(const T& item)
        {
            items_.push_back(item);
        }

        void push(T&& item)
        {
            items_.push_back(std::move(item));
        }

        size_t size() const;

        const_reference_type top() const
        {
            return items_.back();
        }

        void pop()
        {
            items_.pop_back();
        }
    };

    template <typename T, template <typename, typename> class Container, typename TAllocator>
    size_t Stack<T, Container, TAllocator>::size() const
    {
        return items_.size();
    }
} // namespace TemplateTemplateParams

TEST_CASE("class templates with template params")
{
    TemplateTemplateParams::Stack<int, std::deque> stack_int;

    stack_int.push(42);
    stack_int.push(665);
    stack_int.push(15);

    REQUIRE(stack_int.top() == 15);
    stack_int.pop();
    REQUIRE(stack_int.size() == 2);
}

template <typename TValue>
using Dictionary = std::map<std::string, TValue>;

typedef int Integer;
using Real = double;

TEST_CASE("template aliases")
{
    Dictionary<int> dict = { {"one", 1}, {"two", 2} };
    Dictionary<double> math_dict = { { "pi", 3.14 } };
}

template <typename T>
const T pi_v = 3.1415;

TEST_CASE("template variables")
{
    float radius = 2.0;

    auto area = pi_v<float> * pi_v<float> * radius;

}