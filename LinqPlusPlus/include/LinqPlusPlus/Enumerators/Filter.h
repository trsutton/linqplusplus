#ifndef LINQ_PLUSPLUS_FILTER_ENUMERATOR_H
#define LINQ_PLUSPLUS_FILTER_ENUMERATOR_H

#include "Enumerator.h"
#include <functional>

namespace LinqPlusPlus
{
    namespace Enumerators
    {
        template <typename T>
        class Filter : public Enumerator<T>
        {
        public:
            Filter(Enumerator<T>& source, std::function<bool(const T&)> filter)
                : source_(source)
                , filter_(filter)
            {
            }

            Filter(const Filter& other)
                : source_(other.source_)
                , filter_(other.filter_)

            {
            }

            virtual ~Filter()
            {
                filter_ = nullptr;
            }

            Filter& operator=(const Filter& rhs)
            {
                source_ = rhs.source_;
                filter_ = rhs.filter_;
                return *this;
            }

            bool operator==(const Filter& rhs) const
            {
                return this == &rhs || (source_ == rhs.source_ && filter_ == rhs.filter_);
            }

            bool operator!=(const Filter& rhs) const
            {
                return !(*this == rhs);
            }

            Filter& where(std::function<bool(const T&)> filter)
            {
                filter_ = [=](const T& t){ return filter_(t) && filter(t); };
                return *this;
            }

            virtual T& current_ref() override
            {
                return source_.current_ref();
            }

            virtual T current() const override
            {
                return source_.current();
            }

            virtual bool move_next() override
            {
                while (source_.move_next())
                {
                    if (filter_(source_.current_ref()))
                    {
                        return true;
                    }
                }

                return false;
            }

            virtual void reset() override
            {
                source_.reset();
            }

        private:
            Enumerator<T>& source_;
            std::function<bool(const T&)> filter_;
        };
    }
}

#endif
