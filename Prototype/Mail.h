#pragma once

#include <iostream>

#include <string>
#include <fstream>

class Mail
{
public:
	Mail() = default;

	void set_mail_from(char* mail_from);
	void set_rcpt_to(char* rcpt_to);
	void set_subject(char* subject);
	void set_text(char* text);

	const char* get_mail_from() const;
	const char* get_rcpt_to() const;
	const char* get_subject() const;
	const char* get_text() const;

	void SaveToFile();

private:
	std::string mail_from;
	std::string rcpt_to;
	std::string subject;
	std::string text;
};

