#pragma once
#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#include "Mail.h"

#pragma comment(lib, "Ws2_32.lib")

#define MAX_ADDRESS_LENGTH 256
#define SMTP_DATA_TERMINATOR "\r\n.\r\n"
#define FIRST_FOUR_SYMBOLS 4

enum MailSessionStatus
{
	EMPTY,
	ELLO,
	MAIL_FROM, 
	RCPT_TO,
	DATA,
	QUIT
};

enum Responses
{
	WELCOME = 220,
	SERVICE_CLOSING,
	OK = 250,
	START_MAIL = 354,
	SYNTAX_ERROR = 501,
	COMMAND_NOT_IMPLEMENTED,
	BAD_SEQUENSE,
	NO_USER = 550,
	USER_NOT_LOCAL
};

class MailSession
{
public:
	MailSession() = delete;
	MailSession(SOCKET& client_socket);

	const SOCKET& GetSocket() const;

	int SendResponse(int response_type);
	int Processes(char* buf);

private:
	int ProcessNotImplemented(bool arg);
	int ProcessHELO(char* buf);
	int ProcessMAIL(char* buf);
	int ProcessRCPT(char* buf);
	int ProcessDATA(char* buf);
	int SubProcessEmail(char* buf);
	int ProcessQUIT();

	bool ValidAdress(char* buf);
	char* CutAddress(char* buf);

private:
	SOCKET client_socket;
	Mail mail_info;
	int current_status = MailSessionStatus::EMPTY;
};

