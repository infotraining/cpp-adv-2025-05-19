#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <array>

namespace Cpp11
{
    constexpr int factorial(int n)
    {
        return (n == 0) ? 1 : n * factorial(n - 1);
    }
} // namespace Cpp11

constexpr int fibonacci(int n)
{
    if (n == 0 || n == 1)
    {
        return n;
    }

    return fibonacci(n-1) + fibonacci(n-2); 
}

template <size_t N, typename F>
constexpr auto create_lookup_table(F f)
{
    std::array<int, N> lookup_table{};

    for(int i = 0; i < N; ++i)
        lookup_table[i] = f(i);

    return lookup_table;
}

TEST_CASE("constexpr")
{
    constexpr double pi = 3.1415;

    constexpr int x = 5;
    constexpr int f5 = Cpp11::factorial(x);
    std::cout << "5! == " << f5 << "\n";

    int tab[Cpp11::factorial(6)] = {};

    constexpr auto fib10 = fibonacci(10);

    constexpr auto factorial_lookup_table = create_lookup_table<10>(Cpp11::factorial);
    constexpr auto fibonacci_lookup_table = create_lookup_table<20>(fibonacci);
}