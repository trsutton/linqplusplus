#ifndef LINQ_PLUSPLUS_CONTAINER_ENUMERATOR_H
#define LINQ_PLUSPLUS_CONTAINER_ENUMERATOR_H

#include "Enumerator.h"
#include <assert.h>
#include <memory>

namespace LinqPlusPlus
{
    namespace Enumerators
    {
        template < typename T, typename Container >
        class ContainerEnumerator : public Enumerator<T>
        {
        public:
            explicit ContainerEnumerator(const Container& container)
                : Enumerator<T>()
                , container_(container)
                , isReset_(true)
            {
                current_ = container_.end();
            }

            ContainerEnumerator(const ContainerEnumerator& other)
                : Enumerator<T>()
                , container_(other.container_)
                , current_(other.current_)
                , isReset_(other.isReset_)
            {
            }

            virtual ~ContainerEnumerator(){};

            ContainerEnumerator& operator=(const ContainerEnumerator& rhs)
            {
                container_ = rhs.container_;
                current_ = rhs.current_;
                isReset_ = rhs.isReset_;

                return *this;
            }

            bool operator==(const ContainerEnumerator& rhs) const
            {
                return container_ == rhs.container_
                    && current_ == rhs.current_
                    && isReset_ == rhs.isReset_;
            }

            bool operator!=(const ContainerEnumerator& rhs) const
            {
                return !(*this == rhs);
            }

            virtual T& current_ref() override
            {
                assert(!isReset_);
                return *current_;
            }

            virtual T current() const override
            {
                assert(!isReset_);
                return *current_;
            }

            virtual bool can_move_next() const
            {
                return container_.begin() != container_.end() && (isReset_ || current_ != container_.end());
            }

            virtual bool move_next() override
            {
                if (!can_move_next())
                {
                    return false;
                }

                if (isReset_)
                {
                    current_ = container_.begin();
                    isReset_ = false;
                    return true;
                }

                return ++current_ != container_.end();
            }

            virtual void reset() override
            {
                isReset_ = true;
                current_ = container_.end();
            }

        private:
            Container container_;
            typename Container::iterator current_;
            bool isReset_;
        };
    }
}

#endif
