#ifndef LINQ_PLUSPLUS_MAP_ENUMERATOR_H
#define LINQ_PLUSPLUS_MAP_ENUMERATOR_H

#include "Enumerator.h"
#include <functional>
#include <memory>

namespace LinqPlusPlus
{
    namespace Enumerators
    {
        template <typename T, typename U>
        class Map : public Enumerator<U>
        {
        public:
            Map(Enumerator<T>& source, std::function<U (const T&)> map)
                : source_(source)
                , map_(map)
                , cached_()
            {
            }

            Map(const Map& other)
                : source_(other.source_)
                , map_(other.map_)
                , cached_()
            {
            }

            virtual ~Map()
            {
                map_ = nullptr;
            }

            Map& operator=(const Map& rhs)
            {
                source_ = rhs.source_;
                map_ = rhs.map_;
                cached_ = rhs.cached_;
                return *this;
            }

            bool operator==(const Map& rhs) const
            {
                return this == &rhs || (source_ == rhs.source_ && map_ == rhs.map_);
            }

            bool operator!=(const Map& rhs) const
            {
                return !(*this == rhs);
            }

            template <typename V>
            Map<U, V> select(std::function<V (const U&)> selector)
            {
                return Map<U, V>(*this, selector);
            }

            virtual U& current_ref() override
            {
                if (cached_ == nullptr)
                {
                    cached_.reset(new U(map_(source_.current_ref())));
                }

                return *cached_;
            }

            virtual U current() const override
            {
                return map_(source_.current_ref());
            }

            virtual bool move_next() override
            {
                cached_.reset();
                return source_.move_next();
            }

            virtual void reset() override
            {
                source_.reset();
            }

        private:
            Enumerator<T>& source_;
            std::function<U (const T&)> map_;
            std::shared_ptr<U> cached_;
        };
    }
}

#endif
