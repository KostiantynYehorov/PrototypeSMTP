#include <iostream>
#include "SMTPServer.h"

int main()
{
	SMTPServer obj;

	if (auto res_init = obj.Initialize())
	{
		if (auto res_settings = obj.SetSocketSettings())
		{
			obj.ServerStart();
		}
	}

	return 0;
}