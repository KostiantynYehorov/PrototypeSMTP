#pragma once

#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <memory>
#include <thread>
#include <string>

#include "ThreadPool.h"
#include "MailSession.h"

#pragma comment(lib, "Ws2_32.lib")

class SMTPServer
{
public:
	bool Initialize();
	bool SetSocketSettings();
	void ServerStart();

private:
	void AcceptConnections();
	static void WorkWithClient(SOCKET client_socket);

private:
	WSADATA wsa_data;
	SOCKET server_socket;
	SOCKADDR_IN server_info;

	std::unique_ptr<ThreadPool> thread_pool = nullptr;
};

