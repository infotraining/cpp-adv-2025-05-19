#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>

////////////////////////////////////////////////
// simplified implementation of unique_ptr - only moveable type

using Helpers::Gadget;

namespace Explain
{
    template <typename T>
    class unique_ptr
    {
    private:
        T* ptr_;

    public:
        explicit unique_ptr(T* ptr) noexcept
            : ptr_{ptr}
        { }

        unique_ptr(const unique_ptr& other) = delete;
        unique_ptr& operator=(const unique_ptr& other) = delete;

        // move constructor
        unique_ptr(unique_ptr&& other) noexcept : ptr_{other.ptr_}
        {
            other.ptr_ = nullptr;
        }

        // move assignment operator
        unique_ptr& operator=(unique_ptr&& other) noexcept
        {
            if (this != &other)
            {
                unique_ptr<T> temp = std::move(other);
                swap(temp);
            }

            return *this;
        }

        void swap(unique_ptr& other) noexcept
        {
            std::swap(ptr_, other.ptr_);
        }

        T* get() const noexcept
        {
            return ptr_;
        }

        T* release() noexcept
        {
            T* old_value = ptr_;
            ptr_ = nullptr;
            return old_value;
        }

        ~unique_ptr() noexcept
        {
            delete ptr_;
        }

        T& operator*() const noexcept
        {
            return *ptr_;
        }

        T* operator->() const noexcept
        {
            return ptr_;
        }
    };

    // template <typename T>
    // unique_ptr<T> make_unique(int arg1, const std::string& arg2)
    // {
    //     return unique_ptr<T>{new T(arg1, arg2)};
    // }

    // template <typename T>
    // unique_ptr<T> make_unique(int arg1, std::string&& arg2)
    // {
    //     return unique_ptr<T>{new T(arg1, std::move(arg2))};
    // }

    template <typename T, typename TArg1, typename TArg2>
    unique_ptr<T> make_unique(TArg1&& arg1, TArg2&& arg2)
    {
        return unique_ptr<T>{new T(std::forward<TArg1>(arg1), std::forward<TArg2>(arg2))};
    }
} // namespace Explain

Explain::unique_ptr<Gadget> create_gadget()
{
    static int id = 0;
    const int current_id = ++id;
    // Explain::unique_ptr<Gadget> ptr_g{new Gadget(current_id, "Gadget#" + std::to_string(current_id))};
    auto ptr_g = Explain::make_unique<Gadget>(current_id, "Gadget#" + std::to_string(current_id));
    return ptr_g;
}

TEST_CASE("move semantics - unique_ptr")
{
    Explain::unique_ptr<Gadget> ptr_g{new Gadget(1, "ipad")};

    (*ptr_g).use();
    ptr_g->use();

    Explain::unique_ptr<Gadget> ptr_other = std::move(ptr_g); 
    ptr_other->use();

    ptr_other = create_gadget();
    ptr_other->use();

    ptr_other = std::move(ptr_other); // self assignment
}