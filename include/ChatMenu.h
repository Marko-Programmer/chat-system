#pragma once
#include <iostream>
#include "../include/User.h"
#include "../include/utils.h"
#include "../include/DbManager.h"
#include "../include/Message.h"

class ChatMenu
{
private:
	User*& activeUser;
	DbManager* database;


public:
	ChatMenu(User*& activeUser, DbManager* database);

	// Chat menus
	void displayChatMenu();
	void manageChat(const std::string& chatPartner);

	// Chat activity
	void sendMessage(User& activeUser, const std::string& chatPartner);
	void searchByKeyword(User& activeUser, const std::string& chatPartner);
	void deleteMessage(User& activeUser, const std::string& chatPartner);

	// Friends activity
	void addFriend();
	void deleteFriend();

};

