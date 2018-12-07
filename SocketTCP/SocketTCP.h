#pragma once
#include <WinSock2.h>
#include <string>
class IPAddress;

class SocketTCP
{
public:
	enum class State {
		Done, NotReady, Partial, Disconnected, Error
	};
	SocketTCP();
	SocketTCP::State TCPConnect(const IPAddress &remoteAddress, unsigned short remotePort, int timeout = 0);
	SocketTCP::State TCPSend(const void *data, size_t size);
	SocketTCP::State TCPSendString(const std::string s);
	SocketTCP::State TCPReceive(void *data, size_t size, size_t &received);
	SocketTCP::State TCPReceiveString(std::string &s);
	SocketTCP(SocketTCP &socket) = delete;
	~SocketTCP();
private:
	int sock;
	WSADATA wsa;
	State state = State::NotReady;
};