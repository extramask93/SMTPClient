#include "stdafx.h"

#include "SocketTCP.h"
#include "IPAddress.h"

SocketTCP::SocketTCP()
{
	WSAStartup(MAKEWORD(2, 2), &this->wsa);
}

SocketTCP::State SocketTCP::TCPConnect(const IPAddress & remoteAddress, unsigned short remotePort, int timeout)
{
	struct hostent *hent = NULL;
	struct in_addr in_addr;
	struct protoent *pent;
	struct sockaddr_in addr;
	in_addr.S_un.S_addr = htonl(remoteAddress.toInteger());
	pent = getprotobyname("tcp");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(remotePort);
	addr.sin_addr = in_addr;
	memset(addr.sin_zero, 0, 8);
	sock = socket(AF_INET, SOCK_STREAM, pent->p_proto);
	connect(sock,(struct sockaddr *)&addr, sizeof(struct sockaddr));
	state = State::Done;
	return state;
}

SocketTCP::State SocketTCP::TCPSend(const void * data, std::size_t size)
{
	send(sock, (const char*)data, size, 0);
	return State::Done;
}

SocketTCP::State SocketTCP::TCPSendString(const std::string s)
{
	send(sock, (const char*)s.c_str(), s.length(), 0);
	return State::Done;
}

SocketTCP::State SocketTCP::TCPReceive(void * data, std::size_t size, std::size_t & received)
{
	received = recv(sock, (char*)data, size, 0);
	return State::Done;
}
SocketTCP::State SocketTCP::TCPReceiveString(std::string &s)
{
	char buff[256];
	int received;
	received = recv(sock, (char*)buff, sizeof(buff)-1, 0);
	s = std::string(buff, received);
	return State::Done;
}

SocketTCP::~SocketTCP()
{
	closesocket(sock);
	state = State::Disconnected;
}
