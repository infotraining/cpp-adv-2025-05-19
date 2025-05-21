#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <list>

namespace Explain
{
    namespace Generic
    {
        template <typename Iterator, typename Distance>
        void advance(Iterator& it, Distance n)
        {
            while (n-- > 0)
            {
                ++it;
            }
        }
    } // namespace Generic

    namespace TagDispatching
    {
        // ForwardIterator - ++it;
        // BidirectionalIterator - ++it; --it;
        // RandomAccessIterator - ++it; --it; it += n; it -= n;

        namespace Details
        {
            template <typename Iterator, typename Distance>
            void advance_iter_impl(Iterator& x, Distance n, std::input_iterator_tag)
            {
                // complexity - O(N)
                while (n > 0)
                {
                    ++x;
                    --n;
                }

                std::cout << "O(N)\n";
            }

            template <typename Iterator, typename Distance>
            void advance_iter_impl(Iterator& x, Distance n, std::random_access_iterator_tag)
            {
                // complexity - O(1)
                x += n;

                std::cout << "O(1)\n";
            }
        } // namespace Details

        template <typename Iterator, typename Distance>
        void advance(Iterator& it, Distance n)
        {
            Details::advance_iter_impl(it, n, typename std::iterator_traits<Iterator>::iterator_category{});
        }
    } // namespace TagDispatching

    inline namespace SinceCpp17
    {
        template <typename Iterator, typename Distance>
        void advance(Iterator& it, Distance n)
        {
            using IteratorCategory = typename std::iterator_traits<Iterator>::iterator_category;

            if constexpr (std::is_same_v<IteratorCategory, std::random_access_iterator_tag>)
            {
                it += n;

                std::cout << "O(1)\n";
            }
            else
            {
                while (n-- > 0)
                {
                    ++it;
                }

                std::cout << "O(N)\n";
            }
        }
    } // namespace SinceCpp17
} // namespace Explain

TEST_CASE("advance - optimizing efficiency")
{
    std::list<int> lst = {1, 2, 3, 4, 5};

    auto it = lst.begin();

    Explain::advance(it, 2);

    REQUIRE(*it == 3);
}