#include "../include/Message.h"


Message::Message(const std::string& senderUsername, const  std::string& receiverUsername, const  std::string& text, const  std::string& timestamp)
	: senderUsername(senderUsername), receiverUsername(receiverUsername), text(text), timestamp(timestamp) {}


std::string Message::getSenderUsername() const {
	return senderUsername;
}

std::string Message::getReceiverUsername() const {
	return receiverUsername;
}

std::string Message::getText() const {
	return text;
}

std::string Message::getTimestamp() const {
	return timestamp;
}