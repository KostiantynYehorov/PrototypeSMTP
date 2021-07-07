#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include "Mail.h"

#pragma comment(lib, "Ws2_32.lib")

#define MAX_ADDRESS_LENGTH 256
#define SMTP_DATA_TERMINATOR "."

enum MailSessionStatus
{
	EMPTY,
	ELLO,
	MAIL_FROM, 
	RCPT_TO,
	DATA,
	QUIT
};

class MailSession
{
	int ProcessNotImplemented(bool arg);
	int ProcessHELO(char* buf);
	int ProcessMAIL(char* buf);
	int ProcessRCPT(char* buf);
	int ProcessDATA(char* buf);
	int SubProcessEmail(char* buf);
	int ProcessQUIT();

	bool ValidAdress(char* buf);
	char* CutAddress(char* buf);

public:
	MailSession() = delete;
	MailSession(SOCKET& ClientSocket);

	const SOCKET& GetSocket() const;

	int SendResponse(int ResponseType);
	int Processes(char* buf);

private:
	SOCKET ClientSocket;
	Mail MailInfo;
	int CurrentStatus = MailSessionStatus::EMPTY;
};

