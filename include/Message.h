#pragma once
#include <iostream>
#include "../include/User.h"


class Message
{
private:
	std::string senderUsername;
	std::string receiverUsername;
	std::string text;
	std::string timestamp;


public:

	Message(const std::string& senderUsername, const std::string& receiverUsername, const std::string& text, const std::string& timestamp);

	std::string getSenderUsername() const;
	std::string getReceiverUsername() const;
	std::string getText() const;
	std::string getTimestamp() const;




};

