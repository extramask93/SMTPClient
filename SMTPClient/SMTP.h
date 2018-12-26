#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include "SocketTCP.h"
struct Message
{
	std::string sender;
	std::vector<std::string> recs;
	std::string title;
	std::string body;
};

class SMTP
{
public:
	SMTP();
	void Connect(std::string ip, uint16_t port);
	void Login(std::string login, std::string password);
	void SendMessage(Message &message);
	void Disconnect();
	~SMTP();
private:
	SocketTCP sock;
	std::string s;
	void checkErrorCode(std::string response, std::vector<int> good);
	std::string errorCodeToString(int error);
};

