#ifndef LINQ_PLUSPLUS_IENUMERABLE_H
#define LINQ_PLUSPLUS_IENUMERABLE_H

#include "Enumerators/ArrayEnumerator.h"
#include "Enumerators/Combine.h"
#include "Enumerators/Filter.h"
#include "Enumerators/Map.h"
#include <functional>
#include <map>
#include <stdexcept>
#include <stdint.h>
#include <utility>

#define ENUMERABLE_PTR(__T) std::shared_ptr<IEnumerable< __T > >

namespace LinqPlusPlus
{
    template <typename T> class Iterator;

    template <typename T> class GenericEnumerable;

    template <typename T>
    class IEnumerable
    {
    public:
        typedef Iterator<T> iterator;

        virtual ~IEnumerable(){};

        virtual Enumerator<T>& enumerator(bool initialize = true) = 0;

        iterator begin()
        {
            Enumerator<T>& e = enumerator();
            e.move_next();

            Iterator<T> iter = Iterator<T>(e);
            
            return static_cast<iterator>(iter);
        }

        iterator end()
        {
            Iterator<T> iter = Iterator<T>(enumerator(false), true);
            return static_cast<iterator>(iter);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        T aggregate(std::function<T (const T&, const T&)> accumulator)
        {
            if (accumulator == nullptr)
            {
                throw std::runtime_error("An accumulator function is required");
            }

            Enumerator<T>& enumerator = this->enumerator();

            if (!enumerator.move_next())
            {
                throw std::runtime_error("can't aggregate an empty collection");
            }

            T first = enumerator.current();

            return fold(enumerator, first, accumulator);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename TAccumulate>
        TAccumulate aggregate(const TAccumulate& seed, std::function<TAccumulate (const TAccumulate&, const T&)> accumulator)
        {
            if (accumulator == nullptr)
            {
                throw std::runtime_error("An accumulator function is required");
            }

            return fold(enumerator(), seed, accumulator);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template<typename TAccumulate, typename TResult>
        TResult aggregate(const TAccumulate& seed, 
            std::function<TAccumulate (const TAccumulate&, const T&)> accumulator,
            std::function<TResult (const TAccumulate&)> resultSelector)
        {
            TAccumulate accumulated = aggregate(seed, accumulator);
            return resultSelector(accumulated);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool all(std::function<bool(const T&)> predicate)
        {
            if (predicate == nullptr)
            {
                throw std::runtime_error("A predicate is required");
            }

            Enumerator<T>& enumerator = this->enumerator();

            bool success = true;
            while (success && enumerator.move_next())
            {
                success = predicate(enumerator.current_ref());
            }

            return success;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool any()
        {
            return enumerator().move_next();
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool any(std::function<bool(const T&)> predicate)
        {
            if (predicate == nullptr)
            {
                throw std::runtime_error("A predicate is required");
            }

            Enumerator<T>& enumerator = this->enumerator();

            bool found = false;
            while (!found && enumerator.move_next())
            {
                found = predicate(enumerator.current_ref());
            }

            return found;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        double average(std::function<double(const T&)> selector)
        {
            return average<double>(0.0, selector);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        double average(std::function<float(const T&)> selector)
        {
            return average<float>(0.0f, selector);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        double average(std::function<uint8_t(const T&)> selector)
        {
            return average<uint8_t>(0.0f, selector);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        double average(std::function<uint16_t(const T&)> selector)
        {
            return average<uint16_t>(0.0f, selector);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        double average(std::function<uint32_t(const T&)> selector)
        {
            return average<uint32_t>(0.0f, selector);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        double average(std::function<uint64_t(const T&)> selector)
        {
            return average<uint64_t>(0.0f, selector);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        double average(std::function<int8_t(const T&)> selector)
        {
            return average<int8_t>(0.0f, selector);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        double average(std::function<int16_t(const T&)> selector)
        {
            return average<int16_t>(0.0f, selector);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        double average(std::function<int32_t(const T&)> selector)
        {
            return average<int32_t>(0.0f, selector);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        double average(std::function<int64_t(const T&)> selector)
        {
            return average<int64_t>(0.0f, selector);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename U>
        ENUMERABLE_PTR(U) cast()
        {
            auto castOp = [](const T& t){ return static_cast<U>(t); };
            auto e = std::shared_ptr<Enumerator<U> >(new Enumerators::Map<T, U>(enumerator(), castOp));
            return std::shared_ptr<IEnumerable<U> >(new GenericEnumerable<U>(e));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENUMERABLE_PTR(T) concat(ENUMERABLE_PTR(T) other)
        {
            auto e = std::shared_ptr<Enumerator<T> >(new Enumerators::Combine<T>(enumerator(), other->enumerator()));
            return ENUMERABLE_PTR(T)(new GenericEnumerable<T>(e));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool contains(const T& value)
        {
            return any([&](const T& t){ return t == value; });
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        size_t count()
        {
            return aggregate<size_t>(0, [](const size_t& acc, const T&){ return acc + 1; });
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        size_t count(std::function<bool(const T&)> predicate)
        {
            return where(predicate)->count();
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENUMERABLE_PTR(T) default_if_empty()
        {
            return default_if_empty(T());
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENUMERABLE_PTR(T) default_if_empty(const T& defaultValue)
        {
            if (any())
            {
                std::shared_ptr<Enumerator<T> > e(new Enumerators::Map<T, T>(enumerator(), [](const T& t){ return t; }));
                return ENUMERABLE_PTR(T)(new GenericEnumerable<T>(e));
            }

            T singleton[] = { defaultValue };
            
            auto e = std::shared_ptr<Enumerator<T> >(new Enumerators::ArrayEnumerator<T>(singleton, sizeof(singleton) / sizeof(int)));
            return ENUMERABLE_PTR(T)(new GenericEnumerable<T>(e));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENUMERABLE_PTR(T) distinct()
        {
            std::shared_ptr<std::map<T, T> > distinctValues(new std::map<T, T>);

            auto filter = [=](const T& t)
            {       
                if (distinctValues->find(t) != distinctValues->end())
                {
                    return false;
                }

                distinctValues->insert(std::pair<T, T>(t, t));
                return true;
            };

            auto e = std::shared_ptr<Enumerator<T> >(new Enumerators::Filter<T>(enumerator(), filter));

            return ENUMERABLE_PTR(T)(new GenericEnumerable<T>(e));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        T& element_at(const size_t index)
        {
            Enumerator<T>& enumerator = this->enumerator();
            
            if (!enumerator.move_next())
            {
                throw std::out_of_range("index is out of range");
            }

            size_t i = 0;
            while (i < index && enumerator.move_next())
            {
                ++i;
            }

            if (i < index)
            {
                throw std::out_of_range("index is out of range");
            }

            return enumerator.current_ref();
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        T element_at_or_default(const size_t index, const T& defaultValue)
        {
            try
            {
                return element_at(index);
            }
            catch (std::out_of_range&)
            {
                return defaultValue;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENUMERABLE_PTR(T) except(ENUMERABLE_PTR(T) excluded)
        {
            auto excludedMap = excluded->template to_map<T>([](const T& t) { return t; });

            auto keepIfNotExcluded = [=](const T& t)
            {
                return excludedMap.find(t) == excludedMap.end();
            };

            auto e = std::shared_ptr<Enumerator<T> >(new Enumerators::Filter<T>(enumerator(), keepIfNotExcluded));
            return ENUMERABLE_PTR(T)(new GenericEnumerable<T>(e));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENUMERABLE_PTR(T) except(ENUMERABLE_PTR(T) excluded, std::function<bool(const T& x, const T& y)> equals)
        {
            auto keepIfNotExcluded = [=](const T& element)
            {
                return !excluded->any([&](const T& t){ return equals(element, t); });
            };

            auto e = std::shared_ptr<Enumerator<T> >(new Enumerators::Filter<T>(enumerator(), keepIfNotExcluded));
            return ENUMERABLE_PTR(T)(new GenericEnumerable<T>(e));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        T& first()
        {
            try
            {
                return element_at(0);
            }
            catch (std::out_of_range&)
            {
                throw std::runtime_error("Invalid operation: cannot take first from an empty collection");
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        T& first(std::function<bool(const T&)> predicate)
        {
            if (predicate == nullptr)
            {
                throw std::runtime_error("Argument is null: A predicate is required");
            }

            Enumerator<T>& enumerator = this->enumerator();
            while (enumerator.move_next())
            {
                if (predicate(enumerator.current_ref()))
                {
                    return enumerator.current_ref();
                }
            }

            throw std::runtime_error("Invalid operation: no elements in the collection satisfy the given predicate");
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        T first_or_default(const T& defaultValue)
        {
            return element_at_or_default(0, defaultValue);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        T first_or_default(std::function<bool(const T&)> predicate, const T& defaultValue)
        {
            try
            {
                return first(predicate);
            }
            catch (std::runtime_error&)
            {
                return defaultValue;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ENUMERABLE_PTR(T) where(std::function<bool(const T&)> predicate)
        {
            auto e = std::shared_ptr<Enumerator<T> >(new Enumerators::Filter<T>(enumerator(), predicate));
            return ENUMERABLE_PTR(T)(new GenericEnumerable<T>(e));
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename TKey>
        std::map<TKey, T> to_map(std::function<TKey(const T&)> keySelector)
        {
            return to_map<TKey, T>(keySelector, [](const T& t){ return t; });
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename TKey, typename TResult>
        std::map<TKey, TResult> to_map(std::function<TKey(const T&)> keySelector, std::function<TResult(const T&)> resultSelector)
        {
            std::map<TKey, TResult> map;

            std::for_each(begin(), end(), [&](const T& t)
            {
                return map.insert(std::pair<TKey, TResult>(keySelector(t), resultSelector(t)));
            });

            return map;
        }

    private:
        template<typename TAccumulate>
        double average(TAccumulate seed, std::function<TAccumulate(const T&)> selector)
        {
            if (selector == nullptr)
            {
                throw std::runtime_error("selector is required");
            }

            auto accumulator = [&](const std::pair<TAccumulate, int>& acc, const T& t)
            {
                return std::pair<TAccumulate, int>(acc.first + selector(t), acc.second + 1);
            };

            auto resultSelector = [](const std::pair<TAccumulate, int>& acc)
            {
                return static_cast<double>(acc.first) / acc.second;
            };

            return aggregate<std::pair<TAccumulate, int>, TAccumulate>(std::pair<TAccumulate, int>(seed, 0), accumulator, resultSelector);
        }

        template<typename TAccumulate>
        TAccumulate fold(Enumerator<T>& enumerator, const TAccumulate& seed, std::function<TAccumulate (const TAccumulate&, const T&)> accumulator)
        {
            TAccumulate result = seed;

            while (enumerator.move_next())
            {
                result = accumulator(result, enumerator.current_ref());
            }

            return result;
        }
    };

    template <typename T>
    class Iterator : public std::iterator<std::forward_iterator_tag, T>
    {
        friend class IEnumerable<T>;

    public:
        Iterator(const Iterator& other)
            : enumerator_(other.enumerator_)
            , isEnd_(other.isEnd_)
        {
        }

        Iterator& operator++()
        {
            if (!enumerator_.move_next())
            {
                isEnd_ = true;
            }
            
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator tmp(*this);
            operator++();
            return tmp;
        }

        bool operator==(const Iterator& rhs) const
        {
            return (&enumerator_ == &rhs.enumerator_) && ((isEnd_ && rhs.isEnd_));
        }

        bool operator!=(const Iterator& rhs) const
        {
            return !(*this == rhs);
        }

        T& operator*()
        {
            return enumerator_.current_ref();
        }

    private:
        explicit Iterator(Enumerator<T>& enumerator)
            : enumerator_(enumerator)
            , isEnd_(false)
        {
        }

        Iterator(Enumerator<T>& enumerator, bool isEnd)
            : enumerator_(enumerator)
            , isEnd_(isEnd)
        {
        }

        Enumerator<T>& enumerator_;
        bool isEnd_;
    };

    template <typename T>
    class GenericEnumerable: public IEnumerable<T>
    {
    public:
        explicit GenericEnumerable(std::shared_ptr<Enumerator<T> > enumerator)
            : enumerator_(enumerator)
        {
            
        }

        GenericEnumerable(const GenericEnumerable& other)
            : enumerator_(other.enumerator_)
        {
        }

        virtual ~GenericEnumerable(){}

        GenericEnumerable& operator=(const GenericEnumerable& rhs)
        {
            enumerator_ = rhs.enumerator_;
            return *this;
        }

        virtual Enumerator<T>& enumerator(bool initialize = true) override
        {
            if (initialize) enumerator_->reset();
            return *enumerator_;
        }

    private:
        std::shared_ptr<Enumerator<T> > enumerator_;
    };
}

#endif
