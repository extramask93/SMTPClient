#include "pch.h"
#include "SMTP.h"
#include <iostream>
#include "SocketTCP.h"
#include "encoder.h"
#define SERVER "S: "

SMTP::SMTP()
= default;

void SMTP::Connect(std::string ip, uint16_t port)
{
	std::string s;
	char hostname[50];
	std::cout << "Connecting to the " + ip + " : " + std::to_string(port)<<"\n";
	sock.TCPConnect(ip, port);
	sock.TCPReceiveUntil(s);
	checkErrorCode(s, { 220,250 });
	std::cout << SERVER << s << '\n';
	auto result = gethostname(hostname, sizeof(hostname));
	if (result != 0)
		strcpy_s(hostname,sizeof(hostname), "hostname");
	sock.TCPSendString("HELO "+std::string(hostname)+"\r\n");
	sock.TCPReceiveUntil(s);
	checkErrorCode(s, { 250 });
	std::cout << SERVER << s << '\n';
}

void SMTP::Login(std::string login, std::string password)
{
	sock.TCPSendString("AUTH LOGIN\r\n");
	sock.TCPReceiveUntil(s);
	checkErrorCode(s, { 334,250 });
	std::cout << SERVER << s << '\n';
	sock.TCPSendString(B64::encode(login)+"\r\n");
	sock.TCPReceiveUntil(s);
	checkErrorCode(s, { 334,250 });
	std::cout << SERVER << s << '\n';
	sock.TCPSendString(B64::encode(password)+"\r\n");
	sock.TCPReceiveUntil(s);
	checkErrorCode(s, { 235,250 });
	std::cout << SERVER << s << '\n';
}

void SMTP::SendMessageW(Message& message)
{
	sock.TCPSendString("MAIL FROM:<"+message.sender+">\r\n");
	sock.TCPReceiveUntil(s);
	checkErrorCode(s, { 250 });
	std::cout << SERVER << s<<'\n';
	for (auto rcpt : message.recs) {
		sock.TCPSendString("RCPT TO:<" + rcpt + ">\r\n");//250 success, 550 -failure
		sock.TCPReceiveUntil(s);
		checkErrorCode(s, { 250 });
		std::cout << SERVER << s << '\n';
	}

	sock.TCPSendString("DATA\r\n");
	sock.TCPReceiveUntil(s);
	checkErrorCode(s, { 354,250 });
	std::cout << SERVER << s << '\n';//354 on success
	sock.TCPSendString("From :"+message.sender+"\r\n");
	sock.TCPSendString("To :"+message.recs[0]+"\r\n");
	sock.TCPSendString("Subject:"+message.title+"\r\n");
	sock.TCPSendString("\r\n");
	sock.TCPSendString(message.body + "\r\n");
	sock.TCPSendString("\r\n.\r\n");
	sock.TCPReceiveUntil(s);
	checkErrorCode(s, { 250 });
	std::cout << SERVER << s << '\n';//250 on success
}

void SMTP::Disconnect()
{
	sock.TCPSendString("QUIT\r\n");
	sock.TCPReceiveUntil(s);
	std::cout << SERVER << s << '\n';
}


SMTP::~SMTP()
= default;


void SMTP::checkErrorCode(std::string response, std::vector<int> good)
{
	auto code = atoi(response.substr(0, 3).c_str());
	auto it = std::find(good.begin(), good.end(), code);
	if (it == good.end())
		throw std::runtime_error(std::string("Error code returned from the server: ") +
			+response.substr(0, 3).c_str() +
			errorCodeToString(code)
		);
}

std::string SMTP::errorCodeToString(int error)
{
	std::string result = "";
	switch (error)
	{
			case 211: 	result = "System status, or system help reply"                           ;break;
			case 214: 	result = "Help message"													 ;break;
			case 220:	result = "< domain > Service ready"										 ;break;
			case 221:	result = "< domain > Service closing transmission channel"				 ;break;
			case 250: 	result = "Requested mail action okay, completed"						 ;break;
			case 251: 	result = "User not local; will forward to < forward - path>"			 ;break;
			case 252: 	result = "Cannot VRFY user, but will accept message and attempt delivery";break;
			case 354: 	result = "Start mail input; end with <CRLF>. < CRLF>"					 ;break;
			case 421:	result = "< domain > Service not available, closing transmission channel";break;
			case 450: 	result = "Requested mail action not taken: mailbox unavailable"			 ;break;
			case 451: 	result = "Requested action aborted : local error in processing"			 ;break;
			case 452: 	result = "Requested action not taken : insufficient system storage:"	 ;break;
			case 500: 	result = "Syntax error, command unrecognised"							 ;break;
			case 501: 	result = "Syntax error in parameters or arguments"						 ;break;
			case 502: 	result = "Command not implemented"										 ;break;
			case 503: 	result = "Bad sequence of commands"										 ;break;
			case 504: 	result = "Command parameter not implemented"							 ;break;
			case 521:	result = "< domain > does not accept mail(see rfc1846)"					 ;break;
			case 530: 	result = "Access denied(? ? ? a Sendmailism)"							 ;break;
			case 550: 	result = "Requested action not taken: mailbox unavailable"				 ;break;
			case 551: 	result = "User not local; please try < forward - path>"					 ;break;
			case 552: 	result = "Requested mail action aborted : exceeded storage allocation"	 ;break;
			case 553: 	result = "Requested action not taken : mailbox name not allowed"		 ;break;
			case 554: 	result = "Transaction failed"											 ;break;
	}
	return result;
}
