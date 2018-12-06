#include "SocketTCP.h"



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
	in_addr.S_un.S_addr = remoteAddress.toInteger();
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

SocketTCP::State SocketTCP::TCPReceive(void * data, std::size_t size, std::size_t & received)
{
	received = recv(sock, (char*)data, size, 0);
	return State::Done;
}

SocketTCP::~SocketTCP()
{
	closesocket(sock);
	WSACleanup();
	state = State::Disconnected;
}
