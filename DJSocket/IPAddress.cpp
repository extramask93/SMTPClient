#include "IPAddress.h"
#ifdef _MSC_VER
#include <Winsock2.h>
#else
#include <sys/socket.h>
#endif // _MSC_VER

static const IPAddress  None("0.0.0.0");
static const IPAddress 	Any();
static const IPAddress 	LocalHost("localhost");


IPAddress::IPAddress()
{
}

IPAddress::IPAddress(const std::string & address)
{
	IPAddress(address.c_str());
}

IPAddress::IPAddress(const char * address)
{
	hent = gethostbyname(address);
	if (hent == nullptr) {
		throw std::runtime_error(std::string(address) +" is not a valid address");
	}
}

IPAddress::IPAddress(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3)
{
	uint32_t addr = byte0 | (byte1<<8) | (byte2<<16) | (byte3<<24);
	IPAddress(addr);
}

IPAddress::IPAddress(uint32_t address)
{
	hent = gethostbyaddr((char*)address, sizeof(address), AF_INET);
	if (hent == nullptr)
	{
		throw std::runtime_error("invalid ip");
	}
}

std::string IPAddress::toString() const
{
	return std::string(hent->h_name);
}

uint32_t IPAddress::toInteger() const
{
	struct sockaddr_in addr;
	addr.sin_addr = *((struct in_addr *) hent->h_addr);
	return addr.sin_addr.S_un.S_addr;
}
