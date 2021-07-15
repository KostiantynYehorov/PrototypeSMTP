// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "SMTPServer.h"

void SMTPServer::AcceptConnections()
{
	SOCKET client_socket;

	std::cout << std::this_thread::get_id() << "\n";

	while (true)
	{
		SOCKADDR client_info;
		int client_info_len = sizeof(client_info);

		std::cout << "Waiting for incoming connection...\n";

		if (INVALID_SOCKET == (client_socket = accept(server_socket, &client_info, &client_info_len)))
		{
			std::cout << "Error with client socket accepting!\n";
		}

		else
		{
			std::cout << "Accepted new connection. Now creating session thread...\n";
			
			thread_pool->AddTask(WorkWithClient, client_socket);

			//std::thread new_thread([&]() 
			//	{
			//		WorkWithClient(client_socket);
			//	});

			//new_thread.detach();
		}
	}
}

void SMTPServer::WorkWithClient(SOCKET client_socket)
{
	std::cout << std::this_thread::get_id() << "\n";

	MailSession mail_session(client_socket);
	char buf[128];
	ZeroMemory(&buf, sizeof(buf));

	mail_session.SendResponse(220);

	while (true)
	{
		recv(mail_session.GetSocket(), (char*)&buf, sizeof(buf), 0);

		if (221 == mail_session.Processes(buf))
		{
			std::cout << "End of work\n";
			closesocket(mail_session.GetSocket());
			break;
		}

		ZeroMemory(&buf, sizeof(buf));
		Sleep(50);
	}
}

bool SMTPServer::Initialize()
{
	if (auto res = WSAStartup(MAKEWORD(2,2), &wsa_data) != 0)
	{
		std::cout << "Error with winsock initializing!\n";
		return false;
	}

	thread_pool = std::make_unique<ThreadPool>(3);

	return true;
}

bool SMTPServer::SetSocketSettings()
{
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (server_socket == INVALID_SOCKET)
	{
		std::cout << "Error with socket initializing!\n";
		return false;
	}

	ZeroMemory(&server_info, sizeof(server_info));

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(25);
	//ServerInfo.sin_addr = *(LPIN_ADDR)(gethostbyname("localhost")->h_addr_list[0]);
	server_info.sin_addr.S_un.S_addr = inet_addr("192.168.0.106");

	if (bind(server_socket, (sockaddr*)&server_info, sizeof(server_info)) == SOCKET_ERROR)
	{
		std::cout << "Error with socket binding!\n" << WSAGetLastError();
		return false;
	}

	return true;
}

void SMTPServer::ServerStart()
{
	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Error with server starting!\n";
		exit(WSAGetLastError());
	}

	std::cout << "Server started!\n\n";

	AcceptConnections();
}
