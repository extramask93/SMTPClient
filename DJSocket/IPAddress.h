#pragma once
#include <string>
#include "SocektException.h"

class IPAddress
{
public:
	static const IPAddress 	None;
	static const IPAddress 	Any;
	static const IPAddress 	LocalHost;
	IPAddress();
	IPAddress(const std::string &address);
	IPAddress(const char *address);
	IPAddress(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3);
	IPAddress(uint32_t address);
	std::string 	toString() const;
	uint32_t 	toInteger() const;
private:
	struct hostent *hent = nullptr;
};

