#include "Mail.h"

void Mail::SetMailFrom(char* MailFrom)
{
	this->MailFrom = MailFrom;
}

void Mail::SetRcptTo(char* RcptTo)
{
	this->RcptTo = RcptTo;
}

void Mail::SetSubject(char* Subject)
{
	this->Subject = Subject;
}

void Mail::SetText(char* Text)
{
	this->Text += "\n" + static_cast<std::string>(Text);
}

const char* Mail::GetMailFrom() const
{
	return MailFrom.c_str();
}

const char* Mail::GetRcptTo() const
{
	return RcptTo.c_str();
}

const char* Mail::GetSubject() const
{
	return Subject.c_str();
}

const char* Mail::GetText() const
{
	return Text.c_str();
}

void Mail::SaveToFile()
{
	std::ofstream file("email.txt", std::ofstream::app);

	file << "From: " << MailFrom << "\nTo: " << RcptTo << "\nSubject: " << Subject << "\n\n" << Text << "\n\n";

	file.close();
}
