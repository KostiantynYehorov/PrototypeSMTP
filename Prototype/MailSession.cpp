#include "MailSession.h"

int MailSession::ProcessNotImplemented(bool arg)
{
	if (arg)
	{
		return SendResponse(504);
	}

	else
	{
		return SendResponse(502);
	}
}

MailSession::MailSession(SOCKET& client_socket)
{
	this->client_socket = client_socket;
}

int MailSession::SendResponse(int ResponseType)
{
	char buf[100];

	if (ResponseType == 220)
	{
		strcpy(buf, "220 Welcome! \r\n");
	}

	else if (ResponseType == 221)
	{
		strcpy(buf, "221 Service closing transmission channel\r\n");
	}

	else if (ResponseType == 250)
	{
		strcpy(buf, "250 OK\r\n");
	}

	else if (ResponseType == 234)
	{
		strcpy(buf, "234 go ahead\r\n");
	}

	else if (ResponseType == 354)
	{
		strcpy(buf, "354 Start mail input; end with <CRLF>.<CRLF>\r\n");
	}

	else if (ResponseType == 501)
	{
		strcpy(buf, "501 Syntax error in parameters or arguments\r\n");
	}

	else if (ResponseType == 502)
	{
		strcpy(buf, "502 Command not implemented\r\n");
	}

	else if (ResponseType == 503)
	{
		strcpy(buf, "503 Bad sequence of commands\r\n");
	}

	else if (ResponseType == 550)
	{
		strcpy(buf, "550 No such user\r\n");
	}

	else if (ResponseType == 551)
	{
		strcpy(buf, "551 User not local. Can not forward the mail\r\n");
	}
	
	else
	{
		strcpy(buf, "No description\r\n");
	}

	std::cout << "Sending: " <<  buf << "\n";
	send(client_socket, buf, sizeof(buf), 0);

	return ResponseType;
}

const SOCKET& MailSession::GetSocket() const
{
	return client_socket;
}

int MailSession::Processes(char* buf, int len)
{
	if (_strnicmp(buf, "HELO", 4) == 0)
	{
		//return ProcessHELO(buf, len);
	}

	else if (_strnicmp(buf, "EHLO", 4) == 0)
	{
		//return ProcessHELO(buf, len);
	}

	else if (_strnicmp(buf, "MAIL", 4) == 0)
	{
		//return ProcessMAIL(buf, len);
	}

	else if (_strnicmp(buf, "RCPT", 4) == 0)
	{
		//return ProcessRCPT(buf, len);
	}

	else if (_strnicmp(buf, "DATA", 4) == 0)
	{	
		//return ProcessDATA(buf, len);
	}

	else if (_strnicmp(buf, "QUIT", 4) == 0)
	{
		//return ProcessQUIT(buf, len);
	}

	else
	{
		return ProcessNotImplemented(false);
	}
}
