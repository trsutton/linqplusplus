#include "LinqPlusPlus/Enumerable.h"
#include "gtest/gtest.h"

using namespace LinqPlusPlus;
using namespace testing;

#define ENUMERABLE_TEST(__subject, __test_name) TEST(EnumerableTest_ ## __subject, __test_name)

ENUMERABLE_TEST(Iterators, Are_used_to_iterate_over_the_collection)
{
    std::vector<int> values;
    values.push_back(25);
    values.push_back(-25);

    auto collection = Enumerable::from(values);

    std::vector<int>::iterator expected = values.begin();
    IEnumerable<int>::iterator actual = collection->begin();

    EXPECT_EQ(*expected, *actual);
    ++expected; ++actual;
    EXPECT_EQ(*expected, *actual);
    ++expected;  ++actual;
    EXPECT_EQ(collection->end(), actual);
}

ENUMERABLE_TEST(Range, Generates_a_sequence_of_integral_values)
{
    auto range = Enumerable::range<-5, 11>(); 
    EXPECT_EQ(0, range->aggregate([](const int& x, const int& y){ return x + y; }));
}

ENUMERABLE_TEST(Aggregate, Aggregates_the_items_in_a_collection)
{
    std::vector<int> values;
    for (int i = 5; i >= 0; --i)
        values.push_back(i);

    auto collection = Enumerable::from(values);

    int sum = collection->aggregate([](const int& xs, const int& x){ return xs + x; });

    EXPECT_EQ(15, sum);
}

ENUMERABLE_TEST(Aggregate, Aggregates_the_items_in_a_collection_seeding_the_accumulator_with_the_given_value)
{
    std::vector<int> values;
    for (int i = 1; i <= 4; ++i)
        values.push_back(i);

    auto collection = Enumerable::from(values);

    int product = collection->aggregate<int>(100, [](const int& acc, const int& n){return acc * n; });

    EXPECT_EQ(2400, product);
}

ENUMERABLE_TEST(Aggregate, Aggregates_the_items_in_a_collection_with_the_given_seed_and_result_selector)
{
    std::vector<int> values;
    for (int i = 1; i <= 5; ++i)
        values.push_back(i);

    auto collection = Enumerable::from(values);

    int sumAndSquare = collection->aggregate<int, int>(
        0, 
        [](const int& acc, const int& n){return acc + n; }, 
        [](const int& acc) {return acc * acc; });

    EXPECT_EQ(225, sumAndSquare);
}

ENUMERABLE_TEST(Aggregate, Errors_if_an_accumulator_is_not_provided)
{
    std::vector<int> values;
    values.push_back(1);

    auto collection = Enumerable::from(values);

    EXPECT_THROW(collection->aggregate(nullptr), std::runtime_error);
    EXPECT_THROW(collection->aggregate<int>(100, nullptr), std::runtime_error);
}

ENUMERABLE_TEST(Aggregate, Errors_if_the_collection_is_empty)
{
    std::vector<int> values;

    auto collection = Enumerable::from(values);

    EXPECT_THROW(collection->aggregate([](const int& xs, const int& x){ return xs + x; }), std::runtime_error);
}

ENUMERABLE_TEST(All, Determines_if_all_the_elements_of_a_collection_satisfy_the_given_predicate)
{
    int values[] = { 2, 4, 6, 8, 10 };

    auto is_even = [](const int& n){ return n % 2 == 0; };
    auto is_greater_than_five = [](const int& n){ return n > 5; };

    auto collection = Enumerable::from_array(values, sizeof(values) / sizeof(int));

    EXPECT_TRUE(collection->all(is_even));
    EXPECT_FALSE(collection->all(is_greater_than_five));
}

ENUMERABLE_TEST(Any, Determines_if_the_collection_contains_any_elements)
{
    std::deque<int> values;
    values.push_back(1);

    auto emptyCollection = Enumerable::empty<int>();
    auto nonEmptyCollection = Enumerable::from(values);

    EXPECT_FALSE(emptyCollection->any());
    EXPECT_TRUE(nonEmptyCollection->any());
}

ENUMERABLE_TEST(Any, Determines_if_the_collection_contains_any_elements_that_satisfy_the_given_predicate)
{
    char source[] = { 'a', 'b', 'c', 'd' };
    std::vector<char> values(source, source + 4);

    auto collection = Enumerable::from(values);

    EXPECT_TRUE(collection->any([](const char& value){ return value == 'c'; }));
    EXPECT_FALSE(collection->any([](const char& value){ return value == 'e'; }));
}

ENUMERABLE_TEST(Average, Averages_the_items_in_the_collection_using_the_given_selector)
{
    std::vector<int> values;
    values.push_back(25);
    values.push_back(50);
    values.push_back(100);
    values.push_back(125);

    auto collection = Enumerable::from(values);

    std::function<double(const int&)> selector = [](const int& n) { return static_cast<double>(n) / 100; };

    EXPECT_DOUBLE_EQ(0.75, collection->average(selector));
}

ENUMERABLE_TEST(Cast, Casts_each_element_of_the_collection_to_the_given_type)
{
    std::vector<int8_t> values;
    values.push_back(25);
    values.push_back(50);
    values.push_back(100);
    values.push_back(125);

    auto collection = Enumerable::from(values);

    auto results = collection->cast<double>();
    auto expected = values.begin();

    std::for_each(results->begin(), results->end(), [&](const double& val)
    {
        EXPECT_DOUBLE_EQ(static_cast<double>(*expected), val); ++expected;
    });
}

ENUMERABLE_TEST(Concat, Concats_two_sequences)
{
    std::vector<char> h;
    h.push_back('H'); h.push_back('e'); h.push_back('l'); h.push_back('l'); h.push_back('o'); h.push_back(','); h.push_back(' ');

    std::vector<char> w;
    w.push_back('w'); w.push_back('o'); w.push_back('r'); w.push_back('l'); w.push_back('d'); w.push_back('!');

    auto hello = Enumerable::from(h);
    auto world = Enumerable::from(w);

    std::shared_ptr<IEnumerable<char> > result = hello->concat(world);

    EXPECT_EQ(std::string("Hello, world!"), result->aggregate<std::string>("", [](const std::string& str, const char c){ return str + c; }));
}

ENUMERABLE_TEST(Contains, Determines_if_a_collection_contains_a_given_value)
{
    std::vector<int> source;
    source.push_back(1); source.push_back(2); source.push_back(4);

    auto collection = Enumerable::from(source);

    EXPECT_TRUE(collection->contains(2));
    EXPECT_FALSE(collection->contains(3));
}

ENUMERABLE_TEST(Count, Determines_the_number_of_elements_in_a_collection)
{
    std::vector<int> source;
    for(int i = 0; i < 100; ++i)
    {
        source.push_back(i);
    }

    auto collection = Enumerable::from(source);

    EXPECT_EQ(static_cast<size_t>(100), collection->count());
}

ENUMERABLE_TEST(Count, Counts_the_number_of_elements_that_satisfy_the_given_predicate)
{
    std::vector<int> source;
    for (int i = 0; i < 100; ++i)
    {
        source.push_back(i);
    }

    auto collection = Enumerable::from(source);

    EXPECT_EQ(static_cast<size_t>(50), collection->count([](const int& t){ return t % 2 == 0; }));
}

ENUMERABLE_TEST(DefaultIfEmpty, Returns_an_identical_enumerable_collection_if_not_empty)
{
    std::vector<int> source;
    source.push_back(1); source.push_back(2);

    auto collection = Enumerable::from(source);

    auto results = collection->default_if_empty();

    EXPECT_EQ(2, results->count());
}

ENUMERABLE_TEST(DefaultIfEmpty, Returns_a_collection_with_a_single_default_value_if_the_collection_is_empty)
{
    auto collection = Enumerable::empty<int>();

    auto results = collection->default_if_empty();

    EXPECT_EQ(1, results->count());
    EXPECT_EQ(0, *results->begin());
}

ENUMERABLE_TEST(DefaultIfEmpty, Returns_a_collection_with_a_single_element_of_the_given_value_if_the_collection_is_empty)
{
    auto collection = Enumerable::empty<int>();

    auto results = collection->default_if_empty(10);

    EXPECT_EQ(1, results->count());
    EXPECT_EQ(10, *results->begin());
}

ENUMERABLE_TEST(Distinct, Returns_a_distinct_list_of_elements_using_the_equality_operator)
{
    std::vector<char> letters;
    letters.push_back('a'); letters.push_back('a');
    letters.push_back('b');
    letters.push_back('c'); letters.push_back('c'); letters.push_back('c');
    letters.push_back('d');

    std::vector<char> expected;
    expected.push_back('a'); expected.push_back('b'); expected.push_back('c'); expected.push_back('d');

    auto collection = Enumerable::from(letters);

    auto distinct = collection->distinct();

    auto actual = distinct->begin();
    for (auto it = expected.begin(); it != expected.end(); ++it)
    {
        EXPECT_EQ(*it, *actual);
        ++actual;
    }

    EXPECT_TRUE(actual == distinct->end());
}

ENUMERABLE_TEST(ElementAt, Returns_the_element_at_the_given_index)
{
    int elements[] = { 1, 2, 3 };

    auto collection = Enumerable::from_array(elements, 3);

    EXPECT_EQ(2, collection->element_at(1));
    EXPECT_EQ(1, collection->element_at(0));
    EXPECT_EQ(3, collection->element_at(2));
}

ENUMERABLE_TEST(ElementAt, Errors_if_the_given_index_is_out_of_range)
{
    auto emptyCollection = Enumerable::empty<int>();
    EXPECT_THROW(emptyCollection->element_at(0), std::out_of_range);

    char elements[] = { 't', 'e', 's', 't' };
    auto collection = Enumerable::from_array(elements, 4);
    EXPECT_THROW(collection->element_at(4), std::out_of_range);
}

ENUMERABLE_TEST(ElementAtOrDefault, Returns_the_element_at_the_given_index)
{
    int elements[] = { 1, 2, 3 };

    auto collection = Enumerable::from_array(elements, 3);

    EXPECT_EQ(2, collection->element_at_or_default(1, 0));
    EXPECT_EQ(1, collection->element_at_or_default(0, 0));
    EXPECT_EQ(3, collection->element_at_or_default(2, 0));
}

ENUMERABLE_TEST(ElementAtOrDefault, Returns_the_default_value_if_index_out_of_bounds)
{
    auto emptyCollection = Enumerable::empty<int>();
    EXPECT_EQ(-1, emptyCollection->element_at_or_default(0, -1));

    char elements[] = { 't', 'e', 's', 't' };
    auto collection = Enumerable::from_array(elements, 4);
    EXPECT_EQ('?', collection->element_at_or_default(4, '?'));
}

ENUMERABLE_TEST(Except, Returns_the_elements_not_in_the_collection_of_items_to_be_excluded)
{
    char letters[] = { 'x', 'a', 'x', 'e', 'x', 'i', 'x', 'o', 'x', 'u' };
    auto collection = Enumerable::from_array(letters, sizeof(letters));

    char excludedLetters[] = { 'x' };
    auto excluded = Enumerable::from_array(excludedLetters, sizeof(excludedLetters));

    auto result = collection
        ->except(excluded)
        ->aggregate<std::string>("", [](const std::string& acc, const char& c){ return acc + c; });

    EXPECT_EQ(std::string("aeiou"), result);
}

ENUMERABLE_TEST(Except, Returns_the_elements_not_in_the_collection_of_items_to_be_excluded_using_the_given_equality_comparer)
{
    char letters[] = { 'x', 'a', 'X', 'e', 'x', 'i', 'X', 'o', 'x', 'u' };
    auto collection = Enumerable::from_array(letters, sizeof(letters));

    char excludedLetters[] = { 'x' };
    auto excluded = Enumerable::from_array(excludedLetters, sizeof(excludedLetters));

    auto result = collection
        ->except(excluded, [](const char& x, const char& y){ return tolower(x) == tolower(y); })
        ->aggregate<std::string>("", [](const std::string& acc, const char& c){ return acc + c; });

    EXPECT_EQ(std::string("aeiou"), result);
}

ENUMERABLE_TEST(First, Returns_the_first_element_of_a_collection)
{
    bool elements[] = { true, true, false, false };
    auto collection = Enumerable::from_array(elements, sizeof(elements));

    EXPECT_TRUE(collection->first());
}

ENUMERABLE_TEST(First, Returns_the_first_element_of_a_collection_that_satifies_the_given_predicate)
{
    long elements[] = { 1, 2, 3, 4, 5, 6 };
    auto collection = Enumerable::from_array(elements, sizeof(elements) / sizeof(long));

    EXPECT_EQ(4, collection->first([](const int& n){ return n > 3; }));
}

ENUMERABLE_TEST(First, Errors_if_the_collection_is_empty)
{
    auto emptyCollection = Enumerable::empty<int>();
    EXPECT_THROW(emptyCollection->first(), std::runtime_error);
}

ENUMERABLE_TEST(First, Errors_if_no_element_satisfies_the_given_predicate)
{
    int values[] = { 1, 3, 5, 7 };
    auto collection = Enumerable::from_array(values, sizeof(values) / sizeof(int));
    EXPECT_THROW(collection->first([](const int& n){ return n % 2 == 0; }), std::runtime_error);
}

ENUMERABLE_TEST(FirstOrDefault, Returns_the_first_element_of_a_collection)
{
    char elements[] = { 'a', 'b', 'c', 'd' };
    auto collection = Enumerable::from_array(elements, sizeof(elements));

    EXPECT_EQ('a', collection->first_or_default('x'));
}

ENUMERABLE_TEST(FirstOrDefault, Returns_the_first_element_of_a_collection_that_satifies_the_given_predicate)
{
    long elements[] = { 1, 2, 3, 4, 5, 6 };
    auto collection = Enumerable::from_array(elements, sizeof(elements) / sizeof(long));

    EXPECT_EQ(4, collection->first_or_default([](const int& n){ return n > 3; }, 0));
}

ENUMERABLE_TEST(FirstOrDefault, Returns_the_default_value_if_the_collection_is_empty)
{
    auto emptyCollection = Enumerable::empty<int>();
    EXPECT_EQ(0, emptyCollection->first_or_default(0));
}

ENUMERABLE_TEST(FirstOrDefault, Returns_the_default_value_if_no_element_satisfies_the_given_predicate)
{
    int values[] = { 1, 3, 5, 7 };
    auto collection = Enumerable::from_array(values, sizeof(values) / sizeof(int));
    EXPECT_EQ(0, collection->first_or_default([](const int& n){ return n % 2 == 0; }, 0));
}
