#include <iostream>
#include "SMTPServer.h"

int main()
{
	SMTPServer smtp_server;

	if (auto res_init = smtp_server.Initialize())
	{
		if (auto res_settings = smtp_server.SetSocketSettings())
		{
			smtp_server.ServerStart();
		}
	}

	return 0;
}