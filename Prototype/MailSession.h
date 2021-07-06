#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_ADDRESS_LENGTH 256

enum MailSessionStatus
{
	EMPTY,
	ELLO,
	MAIL_FROM, 
	RCPT_TO
};

class MailSession
{
	int ProcessNotImplemented(bool arg);
	int ProcessHELO(char* buf);
	int ProcessMAIL(char* buf);
	int ProcessRCPT(char* buf);
	int ProcessQUIT(char* buf);

	static bool ValidAdress(char* buf);
	char* CutAddress(char* buf);

public:
	MailSession() = delete;
	MailSession(SOCKET& ClientSocket);

	const SOCKET& GetSocket() const;

	int SendResponse(int ResponseType);
	int Processes(char* buf, int len);

private:
	SOCKET ClientSocket;
	int CurrentStatus = MailSessionStatus::EMPTY;
};

