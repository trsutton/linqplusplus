#ifndef LINQ_PLUSPLUS_ARRAY_ENUMERATOR_H
#define LINQ_PLUSPLUS_ARRAY_ENUMERATOR_H

#include "Enumerator.h"
#include <algorithm>
#include <assert.h>
#include <string.h>

namespace LinqPlusPlus
{
    namespace Enumerators
    {
        template <typename T>
        class ArrayEnumerator: public Enumerator<T>
        {
        public:
            ArrayEnumerator(T* arr, size_t size)
                : arr_(nullptr)
                , size_(size)
                , current_(size_)
                , isReset_(true)
            {
                arr_ = new T[size_];
                memcpy(arr_, arr, size_ * sizeof(T));
            }

            ArrayEnumerator(const ArrayEnumerator& other)
                : size_(other.size_)
                , current_(other.current_)
                , isReset_(other.isReset_)
            {
                if (this != &other)
                {
                    delete[] arr_;
                    arr_ = new T[size_];
                    memcpy(arr_, other.arr_, size_ * sizeof(T));
                }
            }

            virtual ~ArrayEnumerator()
            {
                delete[] arr_;
                arr_ = nullptr;
                size_ = 0;
            }

            ArrayEnumerator& operator=(const ArrayEnumerator& rhs)
            {
                if (this != &rhs)
                {
                    delete[] arr_;
                    arr_ = new T[size_];
                    memcpy(arr_, rhs.arr_, rhs.size_ * sizeof(T));

                    size_ = rhs.size_;
                    current_ = rhs.current_;
                    isReset_ = rhs.isReset_;
                }

                return *this;
            }

            T& current_ref() override
            {
                assert(!isReset_);
                return arr_[current_];
            }

            T current() const override
            {
                assert(!isReset_);
                return arr_[current_];
            }

            virtual bool can_move_next() const
            {
                return size_ > 0 && (isReset_ || current_ < size_);
            }

            virtual bool move_next() override
            {
                if (!can_move_next())
                {
                    return false;
                }

                if (isReset_)
                {
                    current_ = 0;
                    isReset_ = false;
                    return true;
                }

                return ++current_ < size_;
            }

            virtual void reset() override
            {
                isReset_ = true;
                current_ = size_;
            }

        private:
            T* arr_;
            size_t size_;
            size_t current_;
            bool isReset_;
        };
    }
}

#endif
