#include "gadget.hpp"

#include <catch2/catch_test_macros.hpp>
#include <deque>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

using namespace Helpers;

////////////////////////////////////////////////////////
///  PERFECT FORWARDING

void have_fun(Gadget& g)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    g.use();
}

void have_fun(const Gadget& cg)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    cg.use();
}

void have_fun(Gadget&& g)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    g.use();
}

namespace WithoutPerfectForwarding
{
    void use(Gadget& g)
    {
        have_fun(g);
    }

    void use(const Gadget& g)
    {
        have_fun(g);
    }

    void use(Gadget&& g)
    {
        have_fun(std::move(g));
    }
} // namespace WithoutPerfectForwarding

namespace PerfectForwarding
{
    template <typename TGadget>
    void use(TGadget&& g)
    {
        have_fun(std::forward<TGadget>(g));

        // if (TGadget is lvalue_ref)
        // {
        //     have_fun(g);
        // }
        // else // TGadget is not lvalue_ref
        // {
        //     have_fun(std::move(g));
        // }
    }
}

TEST_CASE("using gadget")
{
    Gadget g{1, "g"};
    const Gadget cg{2, "const g"};

    using PerfectForwarding::use;

    use(g);
    use(cg);
    use(Gadget{3, "temporary gadget"});
}


template <typename T>
void ref_collapse_1(T& item)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

template <typename T>
void ref_collapse_2(T&& item)
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

TEST_CASE("reference collapsing")
{
    int x = 32;
    ref_collapse_1(x);
    ref_collapse_1<int>(x);
    ref_collapse_1<int&>(x);    
    ref_collapse_1<int&&>(x);

    ref_collapse_2<int&>(x);
}