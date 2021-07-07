#pragma once
#include <iostream>
#include <string>
#include <fstream>

class Mail
{
public:
	Mail() = default;

	void SetMailFrom(char* MailFrom);
	void SetRcptTo(char* RcptTo);
	void SetSubject(char* Subject);
	void SetText(char* Text);

	const char* GetMailFrom() const;
	const char* GetRcptTo() const;
	const char* GetSubject() const;
	const char* GetText() const;

	void SaveToFile();

private:
	std::string MailFrom;
	std::string RcptTo;
	std::string Subject;
	std::string Text;
};

