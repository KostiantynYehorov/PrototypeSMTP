#include "SMTPServer.h"

void SMTPServer::AcceptConnections()
{
	SOCKET client_socket;

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
	int len;
	char buf[2050];

	mailsession.SendResponse(220);

	while (len = recv(mailsession.GetSocket(), buf, sizeof(buf), 0))
	{
		if (221 == mailsession.Processes(buf, sizeof(buf)))
		{
			std::cout << "End of work\n";
			break;
		}
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
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (server_socket == INVALID_SOCKET)
	{
		std::cout << "Error with socket initializing!\n";
		return false;
	}

	ZeroMemory(&server_info, sizeof(server_info));

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(25);
	server_info.sin_addr = *(LPIN_ADDR)(gethostbyname("localhost")->h_addr_list[0]);

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
