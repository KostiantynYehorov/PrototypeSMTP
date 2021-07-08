#include "MailSession.h"

MailSession::MailSession(SOCKET& client_socket)
{
	this->client_socket = client_socket;
}

const SOCKET& MailSession::GetSocket() const
{
	return client_socket;
}

bool MailSession::ValidAdress(char* buf)
{
	return (strlen(buf) > 2 && strlen(buf) < 255 && strchr(buf, '@'));
}

char* MailSession::CutAddress(char* buf)
{
	char address[MAX_ADDRESS_LENGTH];
	ZeroMemory(&address, sizeof(address));

	char* start_addr;
	char* end_addr;

	int addr_len;

	start_addr = strchr(buf, '<');
	start_addr++;

	end_addr = strchr(buf, '>');

	addr_len = end_addr - start_addr;

	strncpy(address, start_addr, addr_len);

	return address;
}

int MailSession::SendResponse(int response_type)
{
	char buf[64];
	ZeroMemory(&buf, sizeof(buf));

	if (response_type == 220)
	{
		strcpy(buf, "220 Welcome!\r\n");
	}

	else if (response_type == 221)
	{
		strcpy(buf, "221 Service closing transmission channel\r\n");
	}

	else if (response_type == 250)
	{
		strcpy(buf, "250 OK\r\n");
	}

	else if (response_type == 234)
	{
		strcpy(buf, "234 go ahead\r\n");
	}

	else if (response_type == 354)
	{
		strcpy(buf, "354 Start mail input; end with <CRLF>.<CRLF>\r\n");
	}

	else if (response_type == 501)
	{
		strcpy(buf, "501 Syntax error in parameters or arguments\r\n");
	}

	else if (response_type == 502)
	{
		strcpy(buf, "502 Command not implemented\r\n");
	}

	else if (response_type == 503)
	{
		strcpy(buf, "503 Bad sequence of commands\r\n");
	}

	else if (response_type == 550)
	{
		strcpy(buf, "550 No such user\r\n");
	}

	else if (response_type == 551)
	{
		strcpy(buf, "551 User not local. Can not forward the mail\r\n");
	}

	else
	{
		strcpy(buf, "No description\r\n");
	}

	std::cout << "Sending: " << buf << "\n";
	send(client_socket, buf, sizeof(buf), 0);

	return response_type;
}

int MailSession::Processes(char* buf)
{
	if (current_status == MailSessionStatus::DATA)
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

	if (current_status != MailSessionStatus::EMPTY)
	{
		return SendResponse(503);
	}

	if (strchr(buf, '.') == NULL)
	{
		return SendResponse(501);
	}

	current_status = MailSessionStatus::ELLO;

	return SendResponse(250);
}

int MailSession::ProcessMAIL(char* buf)
{
	std::cout << "Received 'MAIL FROM'\n";

	if (current_status != MailSessionStatus::ELLO)
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

	current_status = MailSessionStatus::MAIL_FROM;
	mail_info.set_mail_from(address);

	return SendResponse(250);
}

int MailSession::ProcessRCPT(char* buf)
{
	std::cout << "Received 'RCPT TO'\n";

	if (current_status != MailSessionStatus::MAIL_FROM)
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

	current_status = MailSessionStatus::RCPT_TO;
	mail_info.set_rcpt_to(address);

	return SendResponse(250);
}

int MailSession::ProcessDATA(char* buf)
{
	std::cout << "Received 'DATA'\n";

	if (current_status != MailSessionStatus::RCPT_TO)
	{
		return SendResponse(503);
	}

	current_status = MailSessionStatus::DATA;
	return SendResponse(354);
}

int MailSession::SubProcessEmail(char* buf)
{
	mail_info.set_text(buf);

	if (strstr(buf, SMTP_DATA_TERMINATOR))
	{
		std::cout << "Received DATA END\n";
		current_status = MailSessionStatus::QUIT;

		return SendResponse(250);
	}
}

int MailSession::ProcessQUIT()
{
	mail_info.SaveToFile();
	std::cout << "Received 'QUIT'\n";
	return SendResponse(221);
}
