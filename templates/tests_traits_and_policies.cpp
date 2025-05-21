#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <mutex>

/////////////////////////////////////////////////////////////////
// static polymorphism

namespace DynamicPolymorphism
{
    struct Formatter
    {
        virtual std::string format(const std::string& message) const = 0;
    };

    struct UpperCaseFormatter : Formatter
    {
        std::string format(const std::string& message) const override
        {
            std::string result = message;
            std::transform(result.begin(), result.end(),
                result.begin(), [](char c) { return std::toupper(c); });
            return result;
        }
    };

    struct CapitalizeFormatter : Formatter
    {
        std::string format(const std::string& message) const override
        {
            std::string result = message;
            result[0] = std::toupper(result[0]);
            return result;
        }
    };

    struct Logger
    {
        std::unique_ptr<Formatter> formatter_;

        Logger(std::unique_ptr<Formatter> formatter)
            : formatter_(std::move(formatter))
        { }

        void log(const std::string& message)
        {
            std::cout << formatter_->format(message) << std::endl;
        }
    };
} // namespace DynamicPolymorphism

TEST_CASE("polymorhism")
{
    using namespace DynamicPolymorphism;

    Logger logger1(std::make_unique<UpperCaseFormatter>());
    logger1.log("start");

    Logger logger2(std::make_unique<CapitalizeFormatter>());
    logger2.log("stop");
}

namespace StaticPolymorphism
{
    struct UpperCaseFormatter
    {
        std::string format(const std::string& message) const
        {
            std::string result = message;
            std::transform(result.begin(), result.end(),
                result.begin(), [](char c) { return std::toupper(c); });
            return result;
        }
    };

    struct CapitalizeFormatter
    {
        std::string format(const std::string& message) const
        {
            std::string result = message;
            result[0] = std::toupper(result[0]);
            return result;
        }
    };

    template <typename TFormatter = UpperCaseFormatter>
    struct Logger
    {
        TFormatter formatter_;

        Logger() = default;

        Logger(TFormatter formatter)
            : formatter_(std::move(formatter))
        { }

        void log(const std::string& message)
        {
            std::cout << formatter_.format(message) << std::endl;
        }
    };
} // namespace StaticPolymorphism

TEST_CASE("static polymorphism")
{
    using namespace StaticPolymorphism;

    Logger<UpperCaseFormatter> logger1;
    logger1.log("start");

    Logger logger2(CapitalizeFormatter{}); // CTAD - C++17
    logger2.log("stop");
}

/////////////////////////////////////////////////////////////////
// RangeCheckPolicy
//
class ThrowingRangeChecker
{
protected:
    ~ThrowingRangeChecker() = default;

    void check_range(size_t index, size_t size) const
    {
        if (index >= size)
            throw std::out_of_range("Index out of range...");
    }
};

/////////////////////////////////////////////////////////////////
// RangeCheckPolicy
//
class LoggingErrorRangeChecker
{
public:
    void set_log_file(std::ostream& log_file)
    {
        log_ = &log_file;
    }

protected:
    ~LoggingErrorRangeChecker() = default;

    void check_range(size_t index, size_t size) const
    {
        if ((index >= size) && (log_ != nullptr))
            *log_ << "Error: Index out of range. Index="
                  << index << "; Size=" << size << std::endl;

        // std::terminate();
    }

private:
    std::ostream* log_{};
};

/////////////////////////////////////////////////////////////////
// LockingPolicy
//
class NullMutex
{
public:
    void lock()
    {
    }

    void unlock()
    {
    }
};

/////////////////////////////////////////////////////////////////
// LockingPolicy
//
using StdMutex = std::mutex;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
template <
    typename T,
    typename RangeCheckPolicy,
    typename LockingPolicy = NullMutex>
class Vector : public RangeCheckPolicy
{
    std::vector<T> items_;
    using mutex_type = LockingPolicy;
    mutable mutex_type mtx_;

public:
    Vector() = default;

    template <typename U>
    Vector(std::initializer_list<U> il)
        : items_{il}
    {
    }

    bool empty() const
    {
        std::lock_guard<mutex_type> lk{mtx_};
        return items_.empty();
    }

    size_t size() const
    {
        std::lock_guard<mutex_type> lk{mtx_};
        return items_.size();
    }

    const T& at(size_t index) const
    {
        std::lock_guard<mutex_type> lk{mtx_};

        RangeCheckPolicy::check_range(index, items_.size());

        return (index >= items_.size()) ? items_.back() : items_[index];
    }

    void push_back(const T& item)
    {
        std::lock_guard<mutex_type> lk{mtx_};

        items_.push_back(item);
    }
};

TEST_CASE("using policies - Policy Based Design")
{
    Vector<int, ThrowingRangeChecker> vec1 = {1, 2, 3, 4, 5};
    REQUIRE_THROWS_AS(vec1.at(10), std::out_of_range);
    REQUIRE(vec1.at(2) == 3);

    std::stringstream str_out;
    Vector<int, LoggingErrorRangeChecker, StdMutex> vec2 = {1, 2, 3};
    vec2.set_log_file(str_out);
    vec2.at(10);

    std::cout << str_out.str() << "\n";
}