#ifndef LINQ_PLUSPLUS_ENUMERABLE_H
#define LINQ_PLUSPLUS_ENUMERABLE_H

#include "IEnumerable.h"
#include "Enumerators/ArrayEnumerator.h"
#include "Enumerators/ContainerEnumerator.h"
#include "Enumerators/SequenceGenerator.h"
#include <deque>
#include <list>
#include <map>
#include <vector>

namespace LinqPlusPlus
{
    namespace Enumerable
    {
        template <typename T>
        ENUMERABLE_PTR(T) from_array(T* arr, size_t size)
        {
            auto enumerator = std::shared_ptr<Enumerator<T> >(new Enumerators::ArrayEnumerator<T>(arr, size));
            return ENUMERABLE_PTR(T)(new GenericEnumerable<T>(enumerator));
        }

        template<typename T, typename Container>
        ENUMERABLE_PTR(T) from(const Container& container)
        {
            auto enumerator = std::shared_ptr<Enumerator<T> >(new Enumerators::ContainerEnumerator<T, Container>(container));
            return ENUMERABLE_PTR(T)(new GenericEnumerable<T>(enumerator));
        }

        template <typename T>
        ENUMERABLE_PTR(T) from(const std::deque<T>& container)
        {
            return from<T, std::deque<T> >(container);
        }

        template <typename T>
        ENUMERABLE_PTR(T) from(const std::list<T>& container)
        {
            return from<T, std::list<T> >(container);
        }

        template <typename Key, typename T>
        std::shared_ptr<IEnumerable<std::pair<Key, T> > > from(const std::map<Key, T>& container)
        {
            return from<T, std::map<Key, T> >(container);
        }

        template <typename T>
        ENUMERABLE_PTR(T) from(const std::vector<T>& container)
        {
            return from<T, std::vector<T> >(container);
        }

        template <typename T>
        ENUMERABLE_PTR(T) empty()
        {
            return from<T>(std::vector<T>());
        }

        template <int Start, int Count>
        ENUMERABLE_PTR(int) range()
        {
            auto enumerator = std::shared_ptr<Enumerator<int> >(
                new Enumerators::SequenceGenerator<int>(
                    Start - 1,
                    [](const int& n){ return n + 1; },
                    [](const int& n){ return (n + 1) >= (Start + Count); }));

            return ENUMERABLE_PTR(int)(new GenericEnumerable<int>(enumerator));
        }
    }
}

#endif
