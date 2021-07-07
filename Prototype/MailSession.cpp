#include "MailSession.h"

MailSession::MailSession(SOCKET& client_socket)
{
	this->ClientSocket = client_socket;
}

const SOCKET& MailSession::GetSocket() const
{
	return ClientSocket;
}

bool MailSession::ValidAdress(char* buf)
{
	return (strlen(buf) > 2 && strlen(buf) < 255 && strchr(buf, '@'));
}

char* MailSession::CutAddress(char* buf)
{
	char address[MAX_ADDRESS_LENGTH];
	ZeroMemory(&address, sizeof(address));

	char* start_adrr, * end_adrr;
	int addr_len;

	start_adrr = strchr(buf, '<');
	start_adrr++;

	end_adrr = strchr(buf, '>');

	addr_len = end_adrr - start_adrr;

	strncpy(address, start_adrr, addr_len);

	return address;
}

int MailSession::SendResponse(int ResponseType)
{
	char buf[64];
	ZeroMemory(&buf, sizeof(buf));

	if (ResponseType == 220)
	{
		strcpy(buf, "220 Welcome!\r\n");
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

	std::cout << "Sending: " << buf << "\n";
	send(ClientSocket, buf, sizeof(buf), 0);

	return ResponseType;
}

int MailSession::Processes(char* buf)
{
	if (CurrentStatus == MailSessionStatus::DATA)
	{
		return SubProcessEmail(buf);
	}


	if (_strnicmp(buf, "HELO", 4) == 0)
	{
		return ProcessHELO(buf);
	}

	else if (_strnicmp(buf, "EHLO", 4) == 0)
	{
		return ProcessHELO(buf);
	}

	else if (_strnicmp(buf, "MAIL", 4) == 0)
	{
		return ProcessMAIL(buf);
	}

	else if (_strnicmp(buf, "RCPT", 4) == 0)
	{
		return ProcessRCPT(buf);
	}

	else if (_strnicmp(buf, "DATA", 4) == 0)
	{
		return ProcessDATA(buf);
	}

	else if (_strnicmp(buf, "QUIT", 4) == 0)
	{
		return ProcessQUIT();
	}

	else
	{
		return ProcessNotImplemented(false);
	}
}

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

int MailSession::ProcessHELO(char* buf)
{
	std::cout << "Received 'HELO' or 'ELHO'\n";

	if (CurrentStatus != MailSessionStatus::EMPTY)
	{
		return SendResponse(503);
	}

	if (strchr(buf, '.') == NULL)
	{
		return SendResponse(501);
	}

	CurrentStatus = MailSessionStatus::ELLO;

	return SendResponse(250);
}

int MailSession::ProcessMAIL(char* buf)
{
	std::cout << "Received 'MAIL FROM'\n";

	if (CurrentStatus != MailSessionStatus::ELLO)
	{
		return SendResponse(503);
	}

	char* address;
	ZeroMemory(&address, sizeof(address));

	address = CutAddress(buf);

	std::cout << "Message from: " << address << "\n";

	if (!MailSession::ValidAdress(address))
	{
		return SendResponse(501);
	}

	CurrentStatus = MailSessionStatus::MAIL_FROM;
	MailInfo.SetMailFrom(address);

	return SendResponse(250);
}

int MailSession::ProcessRCPT(char* buf)
{
	std::cout << "Received 'RCPT TO'\n";

	if (CurrentStatus != MailSessionStatus::MAIL_FROM)
	{
		return SendResponse(503);
	}

	char* address;
	ZeroMemory(&address, sizeof(address));

	address = CutAddress(buf);

	std::cout << "Message from: " << address << "\n";

	if (!MailSession::ValidAdress(address))
	{
		return SendResponse(501);
	}

	CurrentStatus = MailSessionStatus::RCPT_TO;
	MailInfo.SetRcptTo(address);

	return SendResponse(250);
}

int MailSession::ProcessDATA(char* buf)
{
	std::cout << "Received 'DATA'\n";

	if (CurrentStatus != MailSessionStatus::RCPT_TO)
	{
		return SendResponse(503);
	}

	CurrentStatus = MailSessionStatus::DATA;
	return SendResponse(354);
}

int MailSession::SubProcessEmail(char* buf)
{
	MailInfo.SetText(buf);

	if (strstr(buf, SMTP_DATA_TERMINATOR))
	{
		std::cout << "Received DATA END\n";
		CurrentStatus = MailSessionStatus::QUIT;

		return SendResponse(250);
	}
}

int MailSession::ProcessQUIT()
{
	MailInfo.SaveToFile();
	std::cout << "Received 'QUIT'\n";
	return SendResponse(221);
}
