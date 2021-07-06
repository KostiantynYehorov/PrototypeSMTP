#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

class MailSession
{
	int ProcessNotImplemented(bool arg);

public:
	MailSession() = delete;
	MailSession(SOCKET& client_socket);

	int SendResponse(int ResponseType);
	const SOCKET& GetSocket() const;

	int Processes(char* buf, int len);

private:
	SOCKET client_socket;
};

