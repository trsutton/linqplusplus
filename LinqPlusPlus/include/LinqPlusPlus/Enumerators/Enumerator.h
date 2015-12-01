#ifndef LINQ_PLUSPLUS_ENUMERATOR_H
#define LINQ_PLUSPLUS_ENUMERATOR_H

namespace LinqPlusPlus
{
    template <typename T>
    class Enumerator
    {
    public:
        virtual ~Enumerator(){};

        virtual bool operator==(const Enumerator& other)
        {
            return false;
        }

        virtual T& current_ref() = 0;
        virtual T current() const = 0;

        virtual bool move_next() = 0;

        virtual void reset() = 0;
    };
}

#endif
