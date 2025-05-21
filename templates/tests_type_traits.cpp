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
template <typename T, T v>
struct IntegralConstant 
{
    static constexpr T value = v;
};

template <typename T, T v>
constexpr T IntegralConstant_v = IntegralConstant<T, v>::value; 

static_assert(IntegralConstant_v<char, 'a'> == 'a');

template <bool v>
using BoolConstant = IntegralConstant<bool, v>;

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;

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
struct IsSame : FalseType
{
};

template <typename T>
struct IsSame<T, T> : TrueType
{
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
struct IsPointer
{
    static constexpr bool value = false;
};

template <typename T>
struct IsPointer<T*>
{
    static constexpr bool value = true;
};

template <typename T>
constexpr bool IsPointer_v = IsPointer<T>::value;

template <typename T>
auto get_value(T obj)
{
    if constexpr (std::is_pointer_v<T>)
    {
        assert(obj != nullptr);
        return *obj;
    }
    else
    {
        return obj;
    }
}

TEST_CASE("get_value")
{
    int x = 42;
    REQUIRE(get_value(x) == 42);


    int* ptr = &x;
    REQUIRE(get_value(ptr) == 42);
}

////////////////////////////////////////////////////////////////

template <typename T>
struct RemoveConst
{
    using type = T;
};

template <typename T>
struct RemoveConst<const T>
{
    using type = T;
};

template <typename T>
using RemoveConst_t = typename RemoveConst<T>::type;

TEST_CASE("remove_const")
{
    using T1 = int;
    using T2 = const int;

    using T3 = RemoveConst_t<T2>;

    static_assert(std::is_same_v<T1, T3>);
}