#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <vector>

using Helpers::Gadget;

// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-resource

namespace LegacyCode
{
    // forward declarations
    Gadget* get_gadget(const std::string& name);
    void use(Gadget* g);
    void use_gadget(Gadget* g);

    // definitions
    Gadget* get_gadget(const std::string& name)
    {
        static int id = 665;
        return new Gadget(++id, name);
    }

    void use(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";

        delete g;
    }

    void use_gadget(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }
} // namespace LegacyCode

namespace ModernCpp
{
    std::unique_ptr<Gadget> get_gadget(const std::string& name);
    void use(std::unique_ptr<Gadget> g);
    void use_gadget(Gadget* g);

    std::unique_ptr<Gadget> get_gadget(const std::string& name)
    {
        static int id = 665;
        // return std::unique_ptr<Gadget>(new Gadget(++id, name));
        return std::make_unique<Gadget>(++id, name);
    }

    void use(std::unique_ptr<Gadget> g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }

    void use_gadget(Gadget* g)
    {
        if (g)
            std::cout << "Using " << g->name() << "\n";
    }
} // namespace ModernCpp

TEST_CASE("legacy hell with dynamic memory")
{
    using namespace LegacyCode;

    {
        Gadget* g = get_gadget("ipad");

        use_gadget(g);
    } // memory leak

    {
        use_gadget(get_gadget("ipad"));
    } // memory leak

    {
        Gadget* g = new Gadget(13, "ipad");

        use(g);
        // use_gadget(g);  // UB!!! - use after delete
        // std::cout << g->name() << std::endl; // UB!!! - use after delete
    }

    {
        Gadget* g = get_gadget("ipad");

        use(g);

        // delete g; // UB!!! - second delete
    }
}

TEST_CASE("safe memory management")
{
    using namespace ModernCpp;

    {
        std::unique_ptr<Gadget> g = get_gadget("ipad");

        use_gadget(g.get());
    }

    {
        use(get_gadget("ipad"));
    }

    {
        // std::unique_ptr<Gadget> g{new Gadget(13, "ipad")};
        auto g = std::make_unique<Gadget>(13, "ipad");

        auto ptr_g = std::move(g);

        use(std::move(ptr_g));
    }

    {
        auto g = std::make_unique<Gadget>(13, "ipad");

        use(std::move(g));
    }

    {
        auto g = std::make_unique<Gadget>(13, "ipad");

        LegacyCode::use(g.release());
    }
}

class Owner
{
    std::string name_;
    std::unique_ptr<Gadget> ptr_gadget_;

public:
    Owner(std::string name, std::unique_ptr<Gadget> ptr_g)
        : name_{std::move(name)}
        , ptr_gadget_{std::move(ptr_g)}
    { }

    void use_gadget()
    {
        if (ptr_gadget_)
        {
            std::cout << name_ << " is using " << ptr_gadget_->name() << "\n";
        }
    }
};

TEST_CASE("Owner & unique_ptr")
{
    auto g = std::make_unique<Gadget>(665, "IPad");

    Owner owner{"Jan Kowalski", std::move(g)};
    owner.use_gadget();

    Owner other_owner = std::move(owner);
    other_owner.use_gadget();
}

void may_throw()
{
    //throw 42;
}

TEST_CASE("custom deallocators")
{
    SECTION("Legacy code")
    {
        FILE* f = std::fopen("file.dat", "w+");

        fprintf(f, "text");
        may_throw();

        std::fclose(f);
    }

    SECTION("Modern C++ with Legacy code")
    {
        std::unique_ptr<FILE, int (*)(FILE*)> f{std::fopen("file.dat", "w+"), &std::fclose};

        fprintf(f.get(), "text");
        may_throw();
    }

    SECTION("lambda as deallocator")
    {
        auto deallocator = [](FILE* f) { std::cout << "Closing file\n"; std::fclose(f); };
        std::unique_ptr<FILE, decltype(deallocator)> f{std::fopen("file.dat", "w+"), deallocator};

        fprintf(f.get(), "text");
    }
}

namespace LegacyCode
{
    int* create_array(int n)
    {
        return new int[n];
    }
} // namespace LegacyCpp

TEST_CASE("std::unique_ptr<T[]>")
{
    SECTION("dynamic array")
    {
        int* dynamic_array = new int[100];

        for (int i = 0; i < 100; ++i)
            dynamic_array[i] = i;

        delete[] dynamic_array;
    }

    SECTION("dynamic array")
    {
        std::unique_ptr<int[]> dynamic_array(LegacyCode::create_array(100));

        for (int i = 0; i < 100; ++i)
            dynamic_array[i] = i;
    }
}