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
        explicit unique_ptr(T* ptr)
            : ptr_{ptr}
        { }

        unique_ptr(const unique_ptr& other) = delete;
        unique_ptr& operator=(const unique_ptr& other) = delete;

        // move constructor
        unique_ptr(unique_ptr&& other) : ptr_{other.ptr_}
        {
            other.ptr_ = nullptr;
        }

        // move assignment operator
        unique_ptr& operator=(unique_ptr&& other)
        {
            if (this != &other)
            {
                delete ptr_;

                ptr_ = other.ptr_;
                other.ptr_ = nullptr;
            }

            return *this;
        }

        T* get() const
        {
            return ptr_;
        }

        ~unique_ptr()
        {
            delete ptr_;
        }

        T& operator*() const
        {
            return *ptr_;
        }

        T* operator->() const
        {
            return ptr_;
        }
    };
} // namespace Explain

Explain::unique_ptr<Gadget> create_gadget()
{
    static int id = 0;
    const int current_id = ++id;
    Explain::unique_ptr<Gadget> ptr_g{new Gadget(current_id, "Gadget#" + std::to_string(current_id))};
    return ptr_g;
}

TEST_CASE("move semantics - unique_ptr")
{
    Explain::unique_ptr<Gadget> ptr_g{new Gadget(1, "ipad")};

    (*ptr_g).use();
    ptr_g->use();

    Explain::unique_ptr<Gadget> ptr_other = std::move(ptr_g); // cc
    ptr_other->use();

    ptr_other = create_gadget();
    ptr_other->use();

    ptr_other = std::move(ptr_other); // self assignment
}