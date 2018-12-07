#pragma once
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

class IPAddress
{
public:

	IPAddress();
	IPAddress(const std::string &address);
	IPAddress(const char *address);
	IPAddress(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3);
	~IPAddress();
	IPAddress(uint32_t address);
	std::string 	toString() const;
	uint32_t 	toInteger() const;
private:
	struct hostent *hent = NULL;
	WSADATA wsa;
};

