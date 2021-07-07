#include "SMTPServer.h"

void SMTPServer::AcceptConnections()
{
	SOCKET client_socket;

	while (true)
	{
		SOCKADDR client_info;
		int client_info_len = sizeof(client_info);

		std::cout << "Waiting for incoming connection...\n";

		if (INVALID_SOCKET == (client_socket = accept(ServerSocket, &client_info, &client_info_len)))
		{
			std::cout << "Error with client socket accepting!\n";
		}

		else
		{
			std::cout << "Accepted new connection. Now creating session thread...\n";

			std::thread new_thread([&]() 
				{
					WorkWithClient(client_socket);
				});

			new_thread.detach();
		}
	}
}

void SMTPServer::WorkWithClient(SOCKET& client_socket)
{
	MailSession mailsession(client_socket);
	char buf[128];
	ZeroMemory(&buf, sizeof(buf));

	mailsession.SendResponse(220);

	while (true)
	{
		recv(mailsession.GetSocket(), (char*)&buf, sizeof(buf), 0);

		if (221 == mailsession.Processes(buf))
		{
			std::cout << "End of work\n";
			break;
		}

		if (buf != NULL)
		{
			ZeroMemory(&buf, sizeof(buf));
		}

		Sleep(50);
	}
}

bool SMTPServer::Initialize()
{
	if (auto res = WSAStartup(MAKEWORD(2,2), &wsadata) != 0)
	{
		std::cout << "Error with winsock initializing!\n";
		return false;
	}

	return true;
}

bool SMTPServer::SetSocketSettings()
{
	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (ServerSocket == INVALID_SOCKET)
	{
		std::cout << "Error with socket initializing!\n";
		return false;
	}

	ZeroMemory(&ServerInfo, sizeof(ServerInfo));

	ServerInfo.sin_family = AF_INET;
	ServerInfo.sin_port = htons(25);
	//ServerInfo.sin_addr = *(LPIN_ADDR)(gethostbyname("localhost")->h_addr_list[0]);
	ServerInfo.sin_addr.S_un.S_addr = inet_addr("192.168.0.106");

	if (bind(ServerSocket, (sockaddr*)&ServerInfo, sizeof(ServerInfo)) == SOCKET_ERROR)
	{
		std::cout << "Error with socket binding!\n" << WSAGetLastError();
		return false;
	}

	return true;
}

void SMTPServer::ServerStart()
{
	if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Error with server starting!\n";
		exit(WSAGetLastError());
	}

	std::cout << "Server started!\n\n";

	AcceptConnections();
}
