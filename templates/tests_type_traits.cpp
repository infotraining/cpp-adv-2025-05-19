#include <catch2/catch_test_macros.hpp>
#include <iostream>

int value(int value)
{
    return value;
}

/////////////////////////////////////////////////

template <int N>
struct Value
{
    static constexpr int value = N;
};

template <int N>
constexpr int Value_v = Value<N>::value; 

/////////////////////////////////////////////////

template <typename T>
struct Identity
{
    using type = T;
};

template <typename T>
using Identity_t = typename Identity<T>::type;

/////////////////////////////////////////////////

template <typename T1, typename T2>
struct IsSame
{
    static constexpr bool value = false;
};

template <typename T>
struct IsSame<T, T>
{
    static constexpr bool value = true;
};

template <typename T1, typename T2>
constexpr bool IsSame_v = IsSame<T1, T2>::value;

TEST_CASE("type traits")
{   
    REQUIRE(value(42) == 42);
    REQUIRE(Value_v<42> == 42);

    using T1 = int;
    using T2 = Identity_t<int>;

    if constexpr (std::is_same_v<T1, T2>)
    {
        std::cout << "types are the same\n";
    }
    else
    {
        std::cout << "types are the different\n";
    }
}

///////////////////////////////////////////////////////

// trait IsPointer_v

template <typename T>
auto get_value(T obj)
{
    // return obj;

    // return *obj;
}

TEST_CASE("get_value")
{
    int x = 42;
    REQUIRE(get_value(x) == 42);


    int* ptr = &x;
    REQUIRE(get_value(ptr) == 42);
}