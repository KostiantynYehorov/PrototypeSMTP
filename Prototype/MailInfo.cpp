// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "MailInfo.h"

void MailInfo::set_mail_from(std::string& mail_from)
{
	this->mail_from = mail_from;
}

void MailInfo::set_rcpt_to(std::string& rcpt_to)
{
	this->rcpt_to = rcpt_to;
}

void MailInfo::set_subject(std::string& subject)
{
	this->subject = subject;
}

void MailInfo::set_text(std::string& text)
{
	this->text += "\n" + text;
}

const char* MailInfo::get_mail_from() const
{
	return mail_from.c_str();
}

const char* MailInfo::get_rcpt_to() const
{
	return rcpt_to.c_str();
}

const char* MailInfo::get_subject() const
{
	return subject.c_str();
}

const char* MailInfo::get_text() const
{
	return text.c_str();
}

void MailInfo::SaveToFile()
{
	std::ofstream file("email.txt", std::ofstream::app);

	file << "From: " << mail_from << "\nTo: " << rcpt_to << "\nSubject: " << subject << text << "\n\n";

	file.close();
}
