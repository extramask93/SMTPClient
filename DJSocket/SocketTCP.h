#pragma once
#include "IPAddress.h"
#include <WinSock2.h>

class SocketTCP
{
public:
	enum class State {
		Done, NotReady, Partial, Disconnected, Error
	};
	SocketTCP();
	SocketTCP::State TCPConnect(const IPAddress &remoteAddress, unsigned short remotePort, int timeout = 0);
	SocketTCP::State TCPSend(const void *data, std::size_t size);
	SocketTCP::State TCPReceive(void *data, std::size_t size, std::size_t &received);
	~SocketTCP();
private:
	int sock;
	WSADATA wsa;
	State state = State::NotReady;
};