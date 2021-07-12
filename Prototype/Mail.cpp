// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Mail.h"

void Mail::set_mail_from(char* mail_from)
{
	this->mail_from = mail_from;
}

void Mail::set_rcpt_to(char* rcpt_to)
{
	this->rcpt_to = rcpt_to;
}

void Mail::set_subject(char* subject)
{
	this->subject = subject;
}

void Mail::set_text(char* text)
{
	this->text += "\n" + static_cast<std::string>(text);
}

const char* Mail::get_mail_from() const
{
	return mail_from.c_str();
}

const char* Mail::get_rcpt_to() const
{
	return rcpt_to.c_str();
}

const char* Mail::get_subject() const
{
	return subject.c_str();
}

const char* Mail::get_text() const
{
	return text.c_str();
}

void Mail::SaveToFile()
{
	std::ofstream file("email.txt", std::ofstream::app);

	file << "From: " << mail_from << "\nTo: " << rcpt_to << "\nSubject: " << subject << "\n\n" << text << "\n\n";

	file.close();
}
