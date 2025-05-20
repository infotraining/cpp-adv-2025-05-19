#include "helpers.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>

////////////////////////////////////////////////////////////////////////////
// Data - class with copy & move semantics (user provided implementation)

using namespace Helpers;

class Data
{
    std::string name_;
    int* data_;
    size_t size_;

public:
    using iterator = int*;
    using const_iterator = const int*;

    Data(std::string name, std::initializer_list<int> list)
        : name_{std::move(name)}
        , size_{list.size()}
    {
        data_ = new int[list.size()];
        std::copy(list.begin(), list.end(), data_);

        std::cout << "Data(" << name_ << ")\n";
    }

    // copy semantics
    Data(const Data& other)
        : name_(other.name_)
        , size_(other.size_)
    {
        std::cout << "Data(" << name_ << ": cc)\n";
        data_ = new int[size_];
        std::copy(other.begin(), other.end(), data_);
    }

    Data& operator=(const Data& other)
    {
        Data temp(other);
        swap(temp);

        std::cout << "Data=(" << name_ << ": cc)\n";

        return *this;
    }

    // move semantics
    Data(Data&& other) noexcept : name_(std::move(other.name_)), data_(other.data_), size_(other.size_)
    {
        other.data_ = nullptr;
        other.size_ = 0;

        std::cout << "Data(" << name_ << ": mv)\n";
    }

    Data& operator=(Data&& other) noexcept
    {
        if (this != &other)
        {
            Data temp = std::move(other);
            swap(temp);
        }

        std::cout << "Data=(" << name_ << ": mv)\n";

        return *this;
    }

    ~Data() noexcept
    {
        delete[] data_;
    }

    void swap(Data& other) noexcept
    {
        name_.swap(other.name_);
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    iterator begin() noexcept
    {        
        return data_;
    }

    iterator end() noexcept
    {
        return data_ + size_;
    }

    const_iterator begin() const noexcept
    {
        return data_;
    }

    const_iterator end() const noexcept
    {
        return data_ + size_;
    }
};

Data create_data_set()
{
    Data ds{"data-set-one", {54, 6, 34, 235, 64356, 235, 23}};

    return ds;
}

TEST_CASE("Data & move semantics")
{
    Data ds1{"ds1", {1, 2, 3, 4, 5}};

    Data backup = ds1; // copy
    Helpers::print(backup, "backup");

    Data target = std::move(ds1);

    SECTION("move is static_cast<Data&&>")
    {
        Data target = static_cast<Data&&>(ds1);
    }
    Helpers::print(target, "target");
}

TEST_CASE("pushing to vector")
{
    std::vector<Data> vec;

    vec.push_back(Data{"ds1", {1, 2, 3, 4}});
    vec.push_back(Data{"ds2", {1, 2, 3, 4}});
    vec.push_back(Data{"ds3", {1, 2, 3, 4}});
    vec.push_back(Data{"ds4", {1, 2, 3, 4}});
    vec.push_back(Data{"ds5", {1, 2, 3, 4}});
    vec.push_back(Data{"ds6", {1, 2, 3, 4}});
}

void foo() noexcept
{
}

void bar()
{    
}

TEST_CASE("noexcept")
{
    void (*ptr_fun)() noexcept = nullptr;

    ptr_fun = foo;

    SECTION("since C++17")
    {
        static_assert(not std::is_same_v<decltype(foo), decltype(bar)>);
    }
}