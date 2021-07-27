#pragma once
#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#include "MailInfo.h"

#pragma comment(lib, "Ws2_32.lib")

#define MAX_ADDRESS_LENGTH 256
#define SMTP_DATA_TERMINATOR "\r\n.\r\n"
#define FIRST_FOUR_SYMBOLS 4
#define FIRST_EIGHT_SYMBOLS 8

enum MailSessionStatus
{
	EMPTY,
	EHLO,
	AUTH,
	LOGIN,
	PASSWORD,
	AUTH_SUCCESS,
	MAIL_FROM, 
	RCPT_TO,
	DATA,
	SUBJECT,
	QUIT
};

enum Responses
{
	WELCOME_TO_CLIENT = 220,
	SERVICE_CLOSING,
	LOGIN_SUCCESS = 235,
	OK = 250,
	LOGIN_RCV = 334,
	START_MAIL = 354,
	SYNTAX_ERROR = 501,
	COMMAND_NOT_IMPLEMENTED,
	BAD_SEQUENSE,
	EMAIL_N_RECEIVED,
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
	int ProcessAUTH(char* buf);
	int ProcessMAIL(char* buf);
	int ProcessRCPT(char* buf);
	int ProcessDATA(char* buf);

	int SubProcessLoginRecieve(char* buf);
	int SubProcessPasswordRecieve(char* buf);
	int SubProcessEmail(char* buf);
	int SubProcessSubject(char* buf);

	int ProcessQUIT();

	bool ValidAdress(char* buf);

	std::string CutSubject(char* buf);
	std::string CutAddress(char* buf);

private:
	SOCKET client_socket;
	MailInfo mail_info;
	int current_status = MailSessionStatus::EMPTY;
};

