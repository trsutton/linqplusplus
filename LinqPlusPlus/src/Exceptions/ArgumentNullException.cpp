#include "LinqPlusPlus/Exceptions/ArgumentNullException.h"

ArgumentNullException::ArgumentNullException(const std::string& name)
    : std::runtime_error("Argument is null: ")
    , name_(name)
{
}

ArgumentNullException::ArgumentNullException(const ArgumentNullException& other)
    : std::runtime_error(other)
    , name_(other.name_)
{
}

ArgumentNullException::~ArgumentNullException()
{
}

ArgumentNullException& ArgumentNullException::operator=(const ArgumentNullException& rhs)
{
    name_ = rhs.name_;
    return *this;
}

const char* ArgumentNullException::what() const throw()
{
    return (std::runtime_error::what() + name_).c_str();
}
