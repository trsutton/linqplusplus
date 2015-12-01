#ifndef ARGUMENT_NULL_EXCEPTION_H
#define ARGUMENT_NULL_EXCEPTION_H

#include <stdexcept>
#include <string>

class ArgumentNullException: public std::runtime_error
{
public:
	ArgumentNullException(const std::string& name);
    ArgumentNullException(const ArgumentNullException&);
	virtual ~ArgumentNullException();

    ArgumentNullException& operator=(const ArgumentNullException&);

	virtual const char* what() const throw();

private:
    std::string name_;
};

#endif
