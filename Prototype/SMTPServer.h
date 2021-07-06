#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <iostream>
#include "MailSession.h"

#pragma comment(lib, "Ws2_32.lib")

class SMTPServer
{
	void AcceptConnections();
	void WorkWithClient(SOCKET& client_socket);

public:
	bool Initialize();
	bool SetSocketSettings();
	void ServerStart();

private:
	WSADATA wsadata;
	SOCKET server_socket;
	SOCKADDR_IN server_info;
};

