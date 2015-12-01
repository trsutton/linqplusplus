#ifndef LINQ_PLUSPLUS_COMBINE_ENUMERATOR_H
#define LINQ_PLUSPLUS_COMBINE_ENUMERATOR_H

#include "Enumerator.h"
#include <assert.h>

namespace LinqPlusPlus
{
    namespace Enumerators
    {
        template<typename T>
        class Combine: public Enumerator<T>
        {
        public:
            Combine(Enumerator<T>& first, Enumerator<T>& second)
                : first_(first)
                , second_(second)
                , active_(nullptr)
            {
            }

            Combine(const Combine& other)
                : first_(other.first_)
                , second_(other.second_)
                , active_(other.active_)
            {
            }

            virtual ~Combine(){}

            Combine& operator=(const Combine& rhs)
            {
                first_ = rhs.first_;
                second_ = rhs.second_;
                active_ = rhs.active_;
                return *this;
            }

            virtual T& current_ref() override
            {
                return active_->current_ref();
            }

            virtual T current() const override
            {
                assert(active_);
                return active_->current();
            }

            virtual bool move_next() override
            {
                if(first_.move_next())
                {
                    active_ = &first_;
                    return true;
                }
                
                if (second_.move_next())
                {
                    active_ = &second_;
                    return true;
                }

                active_ = nullptr;
                return false;
            }

            virtual void reset() override
            {
                first_.reset();
                second_.reset();
            }

        private:
            Enumerator<T>& first_;
            Enumerator<T>& second_;
            Enumerator<T>* active_;
        };
    }
}

#endif
