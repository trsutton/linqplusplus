#include "LinqPlusPlus/Enumerators/ContainerEnumerator.h"
#include "gtest/gtest.h"
#include <functional>
#include <vector>
#include <deque>
#include <list>

using namespace LinqPlusPlus;
using namespace LinqPlusPlus::Enumerators;
using namespace testing;

namespace
{

    template <typename T>
    struct EnumeratorTestParameters
    {
        std::function<std::shared_ptr<Enumerator<T> > ()> empty_enumerator;

        std::function<std::shared_ptr<Enumerator<T> > (const T&)> single_item_enumerator;

        std::function<std::shared_ptr<Enumerator<T> > (const T&, const T&, const T&)> multi_item_enumerator;
    };
}

class EnumeratorTest: public TestWithParam< EnumeratorTestParameters<int> >
{
};

TEST_P(EnumeratorTest, Should_not_advance_an_empty_enumerator)
{
    EnumeratorTestParameters<int> parameters = GetParam();

    std::shared_ptr<Enumerator<int> > empty = parameters.empty_enumerator();

    EXPECT_FALSE(empty->move_next());
}

TEST_P(EnumeratorTest, Should_enumerate_a_container_with_a_single_element)
{
    const int value = 100;

    EnumeratorTestParameters<int> parameters = GetParam();

    std::shared_ptr<Enumerator<int> > single = parameters.single_item_enumerator(value);

    ASSERT_TRUE(single->move_next());
    EXPECT_EQ(value, single->current());
    EXPECT_FALSE(single->move_next());
}

TEST_P(EnumeratorTest, Should_enumerate_a_container_with_multiple_elements)
{
    EnumeratorTestParameters<int> parameters = GetParam();

    std::shared_ptr<Enumerator<int> > multi = parameters.multi_item_enumerator(1, 2, 3);

    ASSERT_TRUE(multi->move_next());
    EXPECT_EQ(1, multi->current());

    ASSERT_TRUE(multi->move_next());
    EXPECT_EQ(2, multi->current_ref());

    ASSERT_TRUE(multi->move_next());
    EXPECT_EQ(3, multi->current());

    EXPECT_FALSE(multi->move_next());
}

namespace
{
    template <typename T>
    EnumeratorTestParameters<T> CreateContainerEnumeratorTestParams()
    {
        EnumeratorTestParameters<T> parameters;

        parameters.empty_enumerator = []()
        {
            Enumerator<T>* enumerator = new ContainerEnumerator<T, std::vector<T> >(std::vector<T>());
            return std::shared_ptr<Enumerator<T> >(enumerator);
        };

        parameters.single_item_enumerator = [](const T& value)
        {
            std::deque<T> queue;
            queue.push_back(value);

            Enumerator<T>* enumerator = new ContainerEnumerator<T, std::deque<T> >(queue);

            return std::shared_ptr<Enumerator<T> >(enumerator);
        };

        parameters.multi_item_enumerator = [](const T& value1, const T& value2, const T& value3)
        {
            std::list<T> list;
            list.push_back(value1);
            list.push_back(value2);
            list.push_back(value3);

            Enumerator<T>* enumerator = new ContainerEnumerator<T, std::list<T> >(list);

            return std::shared_ptr<Enumerator<T> >(enumerator);
        };

        return parameters;
    }
}

INSTANTIATE_TEST_CASE_P(
    ContainerEnumeratorTest,
    EnumeratorTest,
    Values<EnumeratorTestParameters<int> >(
        CreateContainerEnumeratorTestParams<int>()
    )
);
