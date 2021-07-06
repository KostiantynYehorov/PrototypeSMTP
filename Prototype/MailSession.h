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

	const SOCKET& GetSocket() const;

	int SendResponse(int ResponseType);
	int Processes(char* buf, int len);

private:
	SOCKET client_socket;
};

