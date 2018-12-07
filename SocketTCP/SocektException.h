#pragma once
#include <exception>
#include <string>
class SocektException : public std::exception
{
public:
	SocektException(const char * message) : std::exception(message) {}
};

