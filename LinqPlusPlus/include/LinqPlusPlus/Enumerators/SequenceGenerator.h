#ifndef LINQ_PLUSPLUS_SEQUENCE_GENERATOR_H
#define LINQ_PLUSPLUS_SEQUENCE_GENERATOR_H

#include "Enumerator.h"
#include <functional>
#include <memory>

namespace LinqPlusPlus
{
    namespace Enumerators
    {
        template <typename T>
        class SequenceGenerator: public Enumerator<T>
        {
        public:
            SequenceGenerator(const T& seed, std::function<T(const T&)> next, std::function<bool (const T&)> done)
                : seed_(seed)
                , current_(seed)
                , next_(next)
                , done_(done)
            {
            }

            SequenceGenerator(const SequenceGenerator& other)
                : seed_(other.seed_)
                , current_(other.current_)
                , next_(other.next_)
                , done_(other.done_)
            {
            }

            virtual ~SequenceGenerator()
            {
                next_ = nullptr;
                done_ = nullptr;
            }

            SequenceGenerator& operator=(const SequenceGenerator& rhs)
            {
                seed_ = rhs.seed_;
                current_ = rhs.current_;
                next_ = rhs.next_;
                done_ = rhs.done_;

                return *this;
            }

            T& current_ref() override
            {
                return current_;
            }

            T current() const override
            {
                return current_;
            }

            bool move_next() override
            {
                if (done_(current_))
                {
                    return false;
                }

                current_ = next_(current_);
                return true;
            }

            void reset() override
            {
                current_ = seed_;
            }

        private:
            T seed_;
            T current_;
            std::function<T(const T&)> next_;
            std::function<bool(const T&)> done_;
        };
    }
}

#endif
